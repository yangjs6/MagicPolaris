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

//! 视图模型
class IMpViewModel
	: public IInterface
{
	GENERATED_BODY()
public:
	//! 返回是否启用
	virtual bool IsEnable(IMpSceneView* SceneView) { return true; }

	//!view更新 (同步)
	virtual void OnUpdateSyn(IMpSceneView* SceneView, float DeltaTime) {}
	//!view更新 (异步)
	virtual void OnUpdateAsyn(IMpSceneView* SceneView, float DeltaTime) {}

	//!准备绘制，返回false表示不支持该视图绘制
	virtual bool OnPreDraw(IMpSceneView* SceneView) { return true; }
	//!结束绘制
	virtual void OnPostDraw(IMpSceneView* SceneView) {}

	//!view回收 (同步)
	virtual void OnRetrieveSyn(IMpSceneView* SceneView, float DeltaTime) {}
	//!view回收 (异步)
	virtual void OnRetrieveAsyn(IMpSceneView* SceneView, float DeltaTime) {}

	//!处理ui事件
	virtual bool OnUIEvent(IMpSceneView* SceneView, const FMpUIEvent& UIEvent) { return false; }
};
