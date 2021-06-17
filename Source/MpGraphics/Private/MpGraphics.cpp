#include "MpGraphics.h"

UMpGraphics::UMpGraphics()
	: bPositionDirty(true)
	, bGeometryDirty(true)
	, bMaterialDirty(true)
{
}

UMpGraphics::~UMpGraphics()
{

}


void UMpGraphics::RebuildGraphics()
{

}

void UMpGraphics::UpdateGraphics(IMpSceneView* pView, IMpGraphicsDrawContext* DrawContext, float DeltaTime)
{
	if (BeginUpdateGraphics(pView, DrawContext, DeltaTime))
	{
		if (bPositionDirty)
		{
			UpdatePosition(pView, DrawContext, DeltaTime);
			bPositionDirty = false;
		}
		if (bGeometryDirty)
		{
			UpdateGeometry(pView, DrawContext, DeltaTime);
			bGeometryDirty = false;
		}
		if (bMaterialDirty)
		{
			UpdateMaterial(pView, DrawContext, DeltaTime);
			bMaterialDirty = false;
		}
	}

	EndUpdateGraphics(pView, DrawContext, DeltaTime);
}

bool UMpGraphics::GetPropertyValue(const FName& PropertyName, FString& PropertyValue) const
{
	return false;
}

bool UMpGraphics::SetPropertyValue(const FName& PropertyName, const FString& PropertyValue)
{
	return false;
}

const FGeoBoundingBox& UMpGraphics::GetBoundingBox()
{
	if (bPositionDirty)
	{
		UpdateBoundingBox();
	}
	return BoundingBox;
}

void UMpGraphics::MarkGraphicsDirty()
{
	bPositionDirty = true;
	bGeometryDirty = true;
	bMaterialDirty = true;
}

void UMpGraphics::MarkPositionDirty()
{
	bPositionDirty = true;
}

void UMpGraphics::MarkGeometryDirty()
{
	bGeometryDirty = true;
}

void UMpGraphics::MarkMaterialDirty()
{
	bMaterialDirty = true;
}
