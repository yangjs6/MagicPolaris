// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

class IMpSceneView;
class IMpViewModel;

class FMpViewModelManager
{
public:
	void UpdateViewModels(IMpSceneView* SceneView, float DeltaTime);

	//!�� ��ͼģ��
	void AttachViewModel(IMpViewModel* ViewModel);
	//!��� ��ͼģ��
	void DetachViewModel(IMpViewModel* ViewModel);
	//!��� ��ͼģ�� ����
	int GetViewModelNum() { return ViewModels.Num(); }
	//!��� ��ͼģ��
	IMpViewModel* GetViewModel(int i) { return ViewModels[i]; }

private:

	TArray<IMpViewModel*> ViewModels;
};