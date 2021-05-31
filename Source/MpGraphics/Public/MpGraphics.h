// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MpTypes.h"
#include "IMpGraphics.h"
#include "MpGraphics.generated.h"

class IMpGraphicsDrawContext;

UCLASS(Abstract)
class UMpGraphicsBase : public UObject, public IMpGraphics
{
	GENERATED_BODY()
public:

	// @property {Property | boolean}[show = true] A boolean Property specifying the visibility of the graphics.
	UPROPERTY(Category = "Graphics", EditAnywhere, meta = (DisplayName = "show"))
	bool Show = true;

public:
	UMpGraphicsBase();
	~UMpGraphicsBase();

	//! 获得图形符号
	virtual IMpSchema* GetSchema() const { return Schema; }

	//!挂载数据源
	virtual void SetVertexSource(IMpVertexSource* pVertexSource);

	//!获得数据源
	virtual IMpVertexSource* GetVertexSource() const;

	//////////////////////////////////////////////////////////////////////////
	//!重构图形
	virtual void RebuildGraphics();

	//!更新图形
	virtual void UpdateGraphics(IMpSceneView* pView, float DeltaTime);

	//! 获得属性值
	virtual bool GetPropertyValue(const FName& PropertyName, FString& PropertyValue) const;

	//! 设置属性值
	virtual bool SetPropertyValue(const FName& PropertyName, const FString& PropertyValue);

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
	void PreDrawGraphics(IMpSceneView* pView, IMpGraphicsDrawContext* DrawContext) {}

	//! 结束绘制图形
	void PostDrawGraphics(IMpSceneView* pView, IMpGraphicsDrawContext* DrawContext) {}

protected:

	bool bPositionDirty;
	bool bGeometryDirty;
	bool bMaterialDirty;

	virtual bool BeginUpdateGraphics(IMpSceneView* pView, float DeltaTime) { return true; }
	virtual void UpdatePosition(IMpSceneView* pView, float DeltaTime) {}
	virtual void UpdateGeometry(IMpSceneView* pView, float DeltaTime) {}
	virtual void UpdateMaterial(IMpSceneView* pView, float DeltaTime) {}
	virtual void EndUpdateGraphics(IMpSceneView* pView, float DeltaTime) {}

	IMpSchema* Schema;
	IMpVertexSource* VertexSource;

	TSharedPtr<IMpVertexSource> DefaultVertexSource;
};
