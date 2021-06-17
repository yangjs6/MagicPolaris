#include "Graphics/MpPolylineGraphics.h"
#include "Generators/MpShapeGenerator.h"


UMpPolylineGraphics::UMpPolylineGraphics()
{

}

TSharedPtr<FMeshShapeGenerator> UMpPolylineGraphics::GenarateFillShape()
{
	return nullptr;
	//TSharedPtr<FMpPolylineMeshGenerator> PolylineMeshGenerator = MakeShareable(new FMpPolylineMeshGenerator());
	//PolylineMeshGenerator->Polyline = FOrientedPolyline3d(FVector3d(), FVector3d(Dimensions.X / 2, Dimensions.Y / 2, Dimensions.Z / 2));
	//PolylineMeshGenerator->bScaleUVByAspectRatio = bScaleUVByAspectRatio;
	//PolylineMeshGenerator->Generate();
	//return PolylineMeshGenerator;
}

bool UMpPolylineGraphics::GenarateOutlineShape()
{
	return false;
}
