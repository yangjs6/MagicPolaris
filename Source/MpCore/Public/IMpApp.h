// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "IMpUnknown.h"

class IMpSceneView;
class IMpViewModel;

//! Ӧ�ó�������Ӧ�õ����
class IMpApp : public IMpUnknown
{
public:
	//!�� ������ͼ
	virtual void AttachSceneView(IMpSceneView* SceneView) = 0;
	//!��� ������ͼ
	virtual void DetachSceneView(IMpSceneView* SceneView) = 0;
	//!��� ������ͼ ����
	virtual int GetSceneViewNum() = 0;
	//!��� ������ͼ
	virtual IMpSceneView* GetSceneView(int i) = 0;

	//!�� ��ͼģ��
	virtual void AttachViewModel(IMpViewModel* ViewModel) = 0;
	//!��� ��ͼģ��
	virtual void DetachViewModel(IMpViewModel* ViewModel) = 0;
	//!��� ��ͼģ�� ����
	virtual int GetViewModelNum() = 0;
	//!��� ��ͼģ��
	virtual IMpViewModel* GetViewModel(int i) = 0;

};
