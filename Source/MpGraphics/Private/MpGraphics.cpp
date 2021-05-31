#include "MpGraphics.h"
#include "MpCoreAPI.h"

UMpGraphicsBase::UMpGraphicsBase()
	: bPositionDirty(true)
	, bGeometryDirty(true)
	, bMaterialDirty(true)
	, Schema(nullptr)
	, VertexSource(nullptr)
{
	DefaultVertexSource = FMpCoreAPI::CreateVertexSource();
}

UMpGraphicsBase::~UMpGraphicsBase()
{

}

void UMpGraphicsBase::SetVertexSource(IMpVertexSource* pVertexSource)
{
	VertexSource = pVertexSource;
}

IMpVertexSource* UMpGraphicsBase::GetVertexSource() const
{
	if (VertexSource)
	{
		return VertexSource;
	}
	return DefaultVertexSource.Get();
}

void UMpGraphicsBase::RebuildGraphics()
{

}

void UMpGraphicsBase::UpdateGraphics(IMpSceneView* pView, float DeltaTime)
{
	if (BeginUpdateGraphics(pView, DeltaTime))
	{
		if (bPositionDirty)
		{
			UpdatePosition(pView, DeltaTime);
			bPositionDirty = false;
		}
		if (bGeometryDirty)
		{
			UpdateGeometry(pView, DeltaTime);
			bGeometryDirty = false;
		}
		if (bMaterialDirty)
		{
			UpdateMaterial(pView, DeltaTime);
			bMaterialDirty = false;
		}
	}

	EndUpdateGraphics(pView, DeltaTime);
}

bool UMpGraphicsBase::GetPropertyValue(const FName& PropertyName, FString& PropertyValue) const
{
	return false;
}

bool UMpGraphicsBase::SetPropertyValue(const FName& PropertyName, const FString& PropertyValue)
{
	return false;
}

void UMpGraphicsBase::MarkGraphicsDirty()
{
	bPositionDirty = true;
	bGeometryDirty = true;
	bMaterialDirty = true;
}

void UMpGraphicsBase::MarkPositionDirty()
{
	bPositionDirty = true;
}

void UMpGraphicsBase::MarkGeometryDirty()
{
	bGeometryDirty = true;
}

void UMpGraphicsBase::MarkMaterialDirty()
{
	bMaterialDirty = true;
}
