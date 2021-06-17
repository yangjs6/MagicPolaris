#include "Graphics/MpBoxGraphics.h"
#include "Generators/MpShapeGenerator.h"


UMpBoxGraphics::UMpBoxGraphics()
{

}

TSharedPtr<FMeshShapeGenerator> UMpBoxGraphics::GenarateFillShape()
{
	TSharedPtr<FMpBoxMeshGenerator> BoxMeshGenerator = MakeShareable(new FMpBoxMeshGenerator());
	BoxMeshGenerator->Box = FOrientedBox3d(FVector3d(), FVector3d(Dimensions.X / 2, Dimensions.Y / 2, Dimensions.Z / 2));
	BoxMeshGenerator->bScaleUVByAspectRatio = bScaleUVByAspectRatio;
	BoxMeshGenerator->Generate();
	return BoxMeshGenerator;
}

bool UMpBoxGraphics::GenarateOutlineShape()
{
	return false;
}
