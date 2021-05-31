// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "IMpUnknown.h"

class IMpSceneView;
class FMpUIEvent;

class IMpViewModel : public IMpUnknown
{
public:
	//!������ͼ������false��ʾ��֧�ָ���ͼ
	virtual bool OnAttachView(IMpSceneView* SceneView) { return true; }
	//!ȡ������ͼ������
	virtual void OnDetachView(IMpSceneView* SceneView) {}

	//!view���� (ͬ��)
	virtual void OnUpdateSyn(IMpSceneView* SceneView, float DeltaTime) {}
	//!view���� (�첽)
	virtual void OnUpdateAsyn(IMpSceneView* SceneView, float DeltaTime) {}
	//!view���� (ͬ��)
	virtual void OnRetrieveSyn(IMpSceneView* SceneView, float DeltaTime) {}
	//!view���� (�첽)
	virtual void OnRetrieveAsyn(IMpSceneView* SceneView, float DeltaTime) {}

	//!׼�����ƣ�����false��ʾ��֧�ָ���ͼ����
	virtual bool OnPreDraw(IMpSceneView* SceneView) { return true; }
	//!��������
	virtual void OnPostDraw(IMpSceneView* SceneView) {}

	//!����ui�¼�
	virtual bool OnUIEvent(IMpSceneView* SceneView, const FMpUIEvent& UIEvent) { return false; }
};