#include "Graphics/MpShapeGraphics.h"
#include "IMpSceneView.h"

void UMpShapeGraphics::UpdateGeometry(IMpSceneView* pView, float DeltaTime)
{
	if (bFill)
	{
		GenarateFillShape();
	}

	if (bOutline)
	{
		GenarateOutlineShape();
	}
}

void UMpShapeGraphicsPoint::UpdatePosition(IMpSceneView* pView, float DeltaTime)
{
	LocalFrame = pView->GetEarthModel()->ComputeFrameFromPosition(Position);
}

void UMpShapeGraphicsPoints::UpdatePosition(IMpSceneView* pView, float DeltaTime)
{

}
