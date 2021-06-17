// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MpTypes.h"
#include "Interfaces/IMpSceneView.h"
#include "Interfaces/IMpGraphics.h"
#include "MpGraphics.generated.h"

UCLASS(Abstract)
class UMpGraphics : public UObject
{
	GENERATED_BODY()
public:

	// @property {Property | boolean}[show = true] A boolean Property specifying the visibility of the graphics.
	UPROPERTY(Category = "Graphics", EditAnywhere, meta = (DisplayName = "show"))
	bool Show = true;

public:
	UMpGraphics();
	~UMpGraphics();

	//////////////////////////////////////////////////////////////////////////
	//!重构图形
	virtual void RebuildGraphics();

	//!更新图形
	virtual void UpdateGraphics(IMpSceneView* pView, IMpGraphicsDrawContext* DrawContext, float DeltaTime);

	//! 获得属性值
	virtual bool GetPropertyValue(const FName& PropertyName, FString& PropertyValue) const;

	//! 设置属性值
	virtual bool SetPropertyValue(const FName& PropertyName, const FString& PropertyValue);

	//! 获得地理空间范围
	virtual const FGeoBoundingBox& GetBoundingBox();

	//! 更新图形所有，不论任何属性更新都可以调用此函数，但效率可能会受影响
	virtual void MarkGraphicsDirty();

	//! 仅更新坐标位置，优化效率
	virtual void MarkPositionDirty();

	//! 仅更新几何图形，不需要更新坐标位置，优化效率
	virtual void MarkGeometryDirty();

	//! 仅更新材质属性，不需要更新几何图形，优化效率
	virtual void MarkMaterialDirty();

public:
	//! 准备绘制图形
	virtual void RegisterToDrawContext(IMpSceneView* pView, IMpGraphicsDrawContext* DrawContext) {}

	//! 结束绘制图形
	virtual void UnRegisterFromDrawContext(IMpSceneView* pView, IMpGraphicsDrawContext* DrawContext) {}

protected:

	FGeoBoundingBox BoundingBox;
	bool bPositionDirty;
	bool bGeometryDirty;
	bool bMaterialDirty;

	virtual void UpdateBoundingBox() {}
	virtual bool BeginUpdateGraphics(IMpSceneView* pView, IMpGraphicsDrawContext* DrawContext, float DeltaTime) { return true; }
	virtual void UpdatePosition(IMpSceneView* pView, IMpGraphicsDrawContext* DrawContext, float DeltaTime) {}
	virtual void UpdateGeometry(IMpSceneView* pView, IMpGraphicsDrawContext* DrawContext, float DeltaTime) {}
	virtual void UpdateMaterial(IMpSceneView* pView, IMpGraphicsDrawContext* DrawContext, float DeltaTime) {}
	virtual void EndUpdateGraphics(IMpSceneView* pView, IMpGraphicsDrawContext* DrawContext, float DeltaTime) {}
};
