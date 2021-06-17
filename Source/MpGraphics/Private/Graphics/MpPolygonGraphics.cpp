#include "Graphics/MpPolygonGraphics.h"
#include "Generators/MpPolygonMeshGenerator.h"


UMpPolygonGraphics::UMpPolygonGraphics()
{

}

TSharedPtr<FMeshShapeGenerator> UMpPolygonGraphics::GenarateFillShape()
{
	TSharedPtr<FMpPolygonMeshGenerator> PolygonMeshGenerator = MakeShareable(new FMpPolygonMeshGenerator());

	TArray<FVector2d> Polygon;
	int NumPoints = UECoordinates.Num();
	Polygon.SetNum(NumPoints);
	for (int i = 0; i < NumPoints; i++)
	{
		Polygon[i] = FVector2d(UECoordinates[i].X, UECoordinates[i].Y);
	}

	PolygonMeshGenerator->Polygon = FPolygon2d(Polygon);
	PolygonMeshGenerator->BottomHeight = Height;
	PolygonMeshGenerator->ExtrudedHeight = ExtrudedHeight;
	PolygonMeshGenerator->bGenerateTop = bCloseTop;
	PolygonMeshGenerator->bGenerateBottom = bCloseBottom;

	PolygonMeshGenerator->Generate();
	return PolygonMeshGenerator;
}

bool UMpPolygonGraphics::GenarateOutlineShape()
{
	return false;
}
