// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UObject/ScriptInterface.h"
#include "MpTypes.h"
#include "IMpViewModel.generated.h"

UINTERFACE()
class MPCORE_API UMpViewModel : public UInterface
{
public:
	GENERATED_BODY()
};

class IMpSceneView;
class FMpUIEvent;

//! ��ͼģ��
class IMpViewModel
	: public IInterface
{
	GENERATED_BODY()
public:
	//! �����Ƿ�����
	virtual bool IsEnable(IMpSceneView* SceneView) { return true; }

	//!view���� (ͬ��)
	virtual void OnUpdateSyn(IMpSceneView* SceneView, float DeltaTime) {}
	//!view���� (�첽)
	virtual void OnUpdateAsyn(IMpSceneView* SceneView, float DeltaTime) {}

	//!׼�����ƣ�����false��ʾ��֧�ָ���ͼ����
	virtual bool OnPreDraw(IMpSceneView* SceneView) { return true; }
	//!��������
	virtual void OnPostDraw(IMpSceneView* SceneView) {}

	//!view���� (ͬ��)
	virtual void OnRetrieveSyn(IMpSceneView* SceneView, float DeltaTime) {}
	//!view���� (�첽)
	virtual void OnRetrieveAsyn(IMpSceneView* SceneView, float DeltaTime) {}

	//!����ui�¼�
	virtual bool OnUIEvent(IMpSceneView* SceneView, const FMpUIEvent& UIEvent) { return false; }
};
