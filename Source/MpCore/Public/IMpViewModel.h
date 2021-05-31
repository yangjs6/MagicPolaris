// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "IMpUnknown.h"

class IMpSceneView;
class FMpUIEvent;

class IMpViewModel : public IMpUnknown
{
public:
	//!连接视图，返回false表示不支持该视图
	virtual bool OnAttachView(IMpSceneView* SceneView) { return true; }
	//!取消和视图的连接
	virtual void OnDetachView(IMpSceneView* SceneView) {}

	//!view更新 (同步)
	virtual void OnUpdateSyn(IMpSceneView* SceneView, float DeltaTime) {}
	//!view更新 (异步)
	virtual void OnUpdateAsyn(IMpSceneView* SceneView, float DeltaTime) {}
	//!view回收 (同步)
	virtual void OnRetrieveSyn(IMpSceneView* SceneView, float DeltaTime) {}
	//!view回收 (异步)
	virtual void OnRetrieveAsyn(IMpSceneView* SceneView, float DeltaTime) {}

	//!准备绘制，返回false表示不支持该视图绘制
	virtual bool OnPreDraw(IMpSceneView* SceneView) { return true; }
	//!结束绘制
	virtual void OnPostDraw(IMpSceneView* SceneView) {}

	//!处理ui事件
	virtual bool OnUIEvent(IMpSceneView* SceneView, const FMpUIEvent& UIEvent) { return false; }
};