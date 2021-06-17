// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Generators/MeshShapeGenerator.h"
#include "Polygon2.h"

/**
 * Generate planar triangulation of a Polygon.
 */
class FMpPolygonMeshGenerator : public FMeshShapeGenerator
{
public:
	/** Polygon to triangulate. If Polygon has self-intersections or degenerate edges, result is undefined. */
	FPolygon2d Polygon;

	/** Normal vector of all vertices will be set to this value. Default is +Z axis. */
	FVector3f Normal;

	/** How to map 2D indices to 3D. Default is (0,1) = (x,y,0). */
	FIndex2i IndicesMap;

	/** Height for vertex Z value */
	double BottomHeight;

	/** ExtrudedHeight for vertex Z value */
	double ExtrudedHeight;

	/**  When false, leaves off the bottom of an extruded polygon open.*/
	bool bGenerateBottom = true;

	/**  When false, leaves off the top of an extruded polygon open.*/
	bool bGenerateTop = false;

public:
	FMpPolygonMeshGenerator();

	/** Initialize the polygon from an array of 2D vertices */
	void SetPolygon(const TArray<FVector2D>& PolygonVerts);

	/** Generate the triangulation */
	virtual FMeshShapeGenerator& Generate() override;


protected:
	void GeneratePolygon(int& VertOffset, int& TriOffset,
		const TArray<FVector2d>& Vertices2D, const TArray<FIndex3i>& Triangles2D, double InHeight, int InPolygonId);
	void GenerateWall(int& VertOffset, int& TriOffset,
		const TArray<FVector2d>& Vertices2D, double InHeight, double InExtrudedHeight, int InPolygonId);

	/** Create vertex at position under IndicesMap, shifted to Origin*/
	inline FVector3d MakeVertex(double x, double y, double z)
	{
		FVector3d v(0, 0, z);
		v[IndicesMap.A] = x;
		v[IndicesMap.B] = y;
		return v;
	}
};