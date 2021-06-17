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
	//!�ع�ͼ��
	virtual void RebuildGraphics();

	//!����ͼ��
	virtual void UpdateGraphics(IMpSceneView* pView, IMpGraphicsDrawContext* DrawContext, float DeltaTime);

	//! �������ֵ
	virtual bool GetPropertyValue(const FName& PropertyName, FString& PropertyValue) const;

	//! ��������ֵ
	virtual bool SetPropertyValue(const FName& PropertyName, const FString& PropertyValue);

	//! ��õ���ռ䷶Χ
	virtual const FGeoBoundingBox& GetBoundingBox();

	//! ����ͼ�����У������κ����Ը��¶����Ե��ô˺�������Ч�ʿ��ܻ���Ӱ��
	virtual void MarkGraphicsDirty();

	//! ����������λ�ã��Ż�Ч��
	virtual void MarkPositionDirty();

	//! �����¼���ͼ�Σ�����Ҫ��������λ�ã��Ż�Ч��
	virtual void MarkGeometryDirty();

	//! �����²������ԣ�����Ҫ���¼���ͼ�Σ��Ż�Ч��
	virtual void MarkMaterialDirty();

public:
	//! ׼������ͼ��
	virtual void RegisterToDrawContext(IMpSceneView* pView, IMpGraphicsDrawContext* DrawContext) {}

	//! ��������ͼ��
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
