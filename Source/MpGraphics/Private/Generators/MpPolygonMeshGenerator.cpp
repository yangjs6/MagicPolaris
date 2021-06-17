// Copyright Epic Games, Inc. All Rights Reserved.

#include "Generators/MpPolygonMeshGenerator.h"
#include "CompGeom/PolygonTriangulation.h"

FMpPolygonMeshGenerator::FMpPolygonMeshGenerator()
{
	Normal = FVector3f::UnitZ();
	IndicesMap = FIndex2i(0, 1);
	BottomHeight = 0;
	ExtrudedHeight = 0;
}



void FMpPolygonMeshGenerator::SetPolygon(const TArray<FVector2D>& PolygonVerts)
{
	Polygon = FPolygon2d();
	int NumVerts = PolygonVerts.Num();
	for (int i = 0; i < NumVerts; ++i)
	{
		Polygon.AppendVertex(FVector2d(PolygonVerts[i]));
	}
}

void OffsetTriangle (FIndex3i& Tri, int VertOffset)
{
	Tri.A += VertOffset;
	Tri.B += VertOffset;
	Tri.C += VertOffset;
}

void FMpPolygonMeshGenerator::GeneratePolygon(int& VertOffset, int& TriOffset,
	const TArray<FVector2d>& Vertices2D, const TArray<FIndex3i>& Triangles2D, double InHeight, int InPolygonId)
{
	int NumVertices = Vertices2D.Num();
	int NumTriangles = Triangles2D.Num();

	FAxisAlignedBox2d BoundingBox(Vertices2D);
	double Width = BoundingBox.Width(), Height = BoundingBox.Height();
	double UVScale = FMath::Max(Width, Height);

	for (int i = 0; i < NumVertices; ++i)
	{
		const FVector2d& Pos = Vertices2D[i];

		int VertIdx = i + VertOffset;

		UVs[VertIdx] = FVector2f(
			(float)((Pos.X - BoundingBox.Min.X) / UVScale),
			(float)((Pos.Y - BoundingBox.Min.Y) / UVScale));
		UVParentVertex[VertIdx] = VertIdx;

		Normals[VertIdx] = Normal;
		NormalParentVertex[VertIdx] = VertIdx;

		Vertices[VertIdx] = MakeVertex(Pos.X, Pos.Y, InHeight);
	}

	for (int i = 0; i < NumTriangles; i++)
	{
		FIndex3i Tri = Triangles2D[i];
		OffsetTriangle(Tri, VertOffset);

		int TriIdx = i + TriOffset;
		SetTriangle(TriIdx, Tri);
		SetTriangleUVs(TriIdx, Tri);
		SetTriangleNormals(TriIdx, Tri);
		SetTrianglePolygon(TriIdx, 0);
	}

	VertOffset += NumVertices;
	TriOffset += NumTriangles;
}


void FMpPolygonMeshGenerator::GenerateWall(int& VertOffset, int& TriOffset, const TArray<FVector2d>& Vertices2D, double InHeight, double InExtrudedHeight, int InPolygonId)
{
	int NumVertices = Vertices2D.Num();


	TArray<double> VerticesUV;
	VerticesUV.SetNum(NumVertices);

	for (int i = 0; i < NumVertices; ++i)
	{
		if (i> 0)
		{
			VerticesUV[i] = VerticesUV[i] + VerticesUV[i - 1];
		}
	}

	double OffsetV = 0;
	for (int i = 0; i < NumVertices + 1; ++i)
	{
		const FVector2d& Pos = Vertices2D[i % NumVertices];
		const FVector2d& Pos1 = Vertices2D[(i + 1) % NumVertices];

		FVector2d Dir = (Pos1 - Pos).Normalized();
		FVector3f PosNormal(Dir.Y, Dir.X, 0);

		int VertIdx = i + VertOffset;

		UVs[VertIdx] = FVector2f(OffsetV, 0);
		UVParentVertex[VertIdx] = VertIdx;
		Normals[VertIdx] = PosNormal;
		NormalParentVertex[VertIdx] = VertIdx;
		Vertices[VertIdx] = MakeVertex(Pos.X, Pos.Y, InHeight);

		VertIdx = i + NumVertices + 1 + VertOffset;

		UVs[VertIdx] = FVector2f(OffsetV, 1);
		UVParentVertex[VertIdx] = VertIdx;
		Normals[VertIdx] = PosNormal;
		NormalParentVertex[VertIdx] = VertIdx;
		Vertices[VertIdx] = MakeVertex(Pos.X, Pos.Y, InHeight + InExtrudedHeight);

		double V = Pos1.Distance(Pos) / InExtrudedHeight;
		OffsetV += V;
	}

	for (int i = 0; i < NumVertices; i++)
	{
		FIndex3i Tri1(i, i + NumVertices, i + 1);
		OffsetTriangle(Tri1, VertOffset);

		int TriIdx = 2 * i + TriOffset;
		SetTriangle(TriIdx, Tri1);
		SetTriangleUVs(TriIdx, Tri1);
		SetTriangleNormals(TriIdx, Tri1);
		SetTrianglePolygon(TriIdx, InPolygonId);


		FIndex3i Tri2( i + 1, i + NumVertices, i + NumVertices + 1);
		OffsetTriangle(Tri2, VertOffset);

		TriIdx = 2 * i + 1 + TriOffset;
		SetTriangle(TriIdx, Tri2);
		SetTriangleUVs(TriIdx, Tri2);
		SetTriangleNormals(TriIdx, Tri2);
		SetTrianglePolygon(TriIdx, InPolygonId);
	}

	VertOffset += (NumVertices + 1) * 2;
	TriOffset += NumVertices * 2;
}

FMeshShapeGenerator& FMpPolygonMeshGenerator::Generate()
{
	const TArray<FVector2d>& Vertices2D = Polygon.GetVertices();
	if (Vertices2D.Num() < 3)
	{
		return *this;
	}

	bool bGenerateWall = true;
	if (ExtrudedHeight <= 0)
	{
		bGenerateBottom = true;
		bGenerateWall = false;
		bGenerateTop = false;
	}

	TArray<FIndex3i> Triangles2D;
	if (bGenerateBottom || bGenerateTop)
	{
		PolygonTriangulation::TriangulateSimplePolygon(Vertices2D, Triangles2D);
	}

	int AllNumVertices = 0;
	int AllNumTriangles = 0;

	if (bGenerateBottom)
	{
		AllNumVertices += Vertices2D.Num();
		AllNumTriangles += Triangles2D.Num();
	}

	if (bGenerateTop)
	{
		AllNumVertices += Vertices2D.Num();
		AllNumTriangles += Triangles2D.Num();
	}

	if (bGenerateWall)
	{
		AllNumVertices += (Vertices2D.Num() + 1) * 2;
		AllNumTriangles += Vertices2D.Num() * 2;
	}

	SetBufferSizes(AllNumVertices, AllNumTriangles, AllNumVertices, AllNumVertices);

	int VertOffset = 0;
	int TriOffset = 0;

	if (bGenerateBottom)
	{
		GeneratePolygon(VertOffset, TriOffset, Vertices2D, Triangles2D, BottomHeight, 0);
	}

	if (bGenerateTop)
	{
		GeneratePolygon(VertOffset, TriOffset, Vertices2D, Triangles2D, BottomHeight + ExtrudedHeight, 1);
	}

	if (bGenerateWall)
	{
		GenerateWall(VertOffset, TriOffset, Vertices2D, BottomHeight, ExtrudedHeight, 2);
	}

	return *this;
}