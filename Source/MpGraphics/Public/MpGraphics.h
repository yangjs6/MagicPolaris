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

	//! ���ͼ�η���
	virtual IMpSchema* GetSchema() const { return Schema; }

	//!��������Դ
	virtual void SetVertexSource(IMpVertexSource* pVertexSource);

	//!�������Դ
	virtual IMpVertexSource* GetVertexSource() const;

	//////////////////////////////////////////////////////////////////////////
	//!�ع�ͼ��
	virtual void RebuildGraphics();

	//!����ͼ��
	virtual void UpdateGraphics(IMpSceneView* pView, float DeltaTime);

	//! �������ֵ
	virtual bool GetPropertyValue(const FName& PropertyName, FString& PropertyValue) const;

	//! ��������ֵ
	virtual bool SetPropertyValue(const FName& PropertyName, const FString& PropertyValue);

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
	void PreDrawGraphics(IMpSceneView* pView, IMpGraphicsDrawContext* DrawContext) {}

	//! ��������ͼ��
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
