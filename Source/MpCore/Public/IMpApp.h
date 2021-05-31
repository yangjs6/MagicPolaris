// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "IMpUnknown.h"

class IMpSceneView;
class IMpViewModel;

//! 应用程序，所有应用的入口
class IMpApp : public IMpUnknown
{
public:
	//!绑定 场景视图
	virtual void AttachSceneView(IMpSceneView* SceneView) = 0;
	//!解除 场景视图
	virtual void DetachSceneView(IMpSceneView* SceneView) = 0;
	//!获得 场景视图 数量
	virtual int GetSceneViewNum() = 0;
	//!获得 场景视图
	virtual IMpSceneView* GetSceneView(int i) = 0;

	//!绑定 视图模型
	virtual void AttachViewModel(IMpViewModel* ViewModel) = 0;
	//!解除 视图模型
	virtual void DetachViewModel(IMpViewModel* ViewModel) = 0;
	//!获得 视图模型 数量
	virtual int GetViewModelNum() = 0;
	//!获得 视图模型
	virtual IMpViewModel* GetViewModel(int i) = 0;

};
