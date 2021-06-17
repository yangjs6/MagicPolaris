// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

class IMpSceneView;
class IMpViewModel;

class FMpViewModelManager
{
public:
	void UpdateViewModels(IMpSceneView* SceneView, float DeltaTime);

	//!绑定 视图模型
	void AttachViewModel(IMpViewModel* ViewModel);
	//!解除 视图模型
	void DetachViewModel(IMpViewModel* ViewModel);
	//!获得 视图模型 数量
	int GetViewModelNum() { return ViewModels.Num(); }
	//!获得 视图模型
	IMpViewModel* GetViewModel(int i) { return ViewModels[i]; }

private:

	TArray<IMpViewModel*> ViewModels;
};