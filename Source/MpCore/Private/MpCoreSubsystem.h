// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "IMpApp.h"
#include "MpCoreSubsystem.generated.h"

class IMpSceneView;
class IMpViewModel;

/**
 * This is the API used to get core information at runtime
 */
UCLASS()
class UMpCoreSubsystem : public UEngineSubsystem, public FTickableGameObject, public IMpApp
{
	GENERATED_BODY()

	UMpCoreSubsystem();

public: // UEngineSubsystem
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

public: // FTickableGameObject
	virtual bool IsTickableInEditor() const override { return true; }

	virtual void Tick(float DeltaTime) override;

	virtual TStatId GetStatId() const override;

public: // IMpApp
	//!�� ������ͼ
	virtual void AttachSceneView(IMpSceneView* SceneView);
	//!��� ������ͼ
	virtual void DetachSceneView(IMpSceneView* SceneView);
	//!��� ������ͼ ����
	virtual int GetSceneViewNum() { return SceneViews.Num(); }
	//!��� ������ͼ
	virtual IMpSceneView* GetSceneView(int i) { return SceneViews[i]; }

	//!�� ��ͼģ��
	virtual void AttachViewModel(IMpViewModel* ViewModel);
	//!��� ��ͼģ��
	virtual void DetachViewModel(IMpViewModel* ViewModel);
	//!��� ��ͼģ�� ����
	virtual int GetViewModelNum() { return ViewModels.Num(); }
	//!��� ��ͼģ��
	virtual IMpViewModel* GetViewModel(int i) { return ViewModels[i]; }

protected:
	void UpdateViewModels(IMpSceneView* SceneView, float DeltaTime);

	TArray<IMpSceneView*> SceneViews;
	TArray<IMpViewModel*> ViewModels;
};
