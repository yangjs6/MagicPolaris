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
	//!绑定 场景视图
	virtual void AttachSceneView(IMpSceneView* SceneView);
	//!解除 场景视图
	virtual void DetachSceneView(IMpSceneView* SceneView);
	//!获得 场景视图 数量
	virtual int GetSceneViewNum() { return SceneViews.Num(); }
	//!获得 场景视图
	virtual IMpSceneView* GetSceneView(int i) { return SceneViews[i]; }

	//!绑定 视图模型
	virtual void AttachViewModel(IMpViewModel* ViewModel);
	//!解除 视图模型
	virtual void DetachViewModel(IMpViewModel* ViewModel);
	//!获得 视图模型 数量
	virtual int GetViewModelNum() { return ViewModels.Num(); }
	//!获得 视图模型
	virtual IMpViewModel* GetViewModel(int i) { return ViewModels[i]; }

protected:
	void UpdateViewModels(IMpSceneView* SceneView, float DeltaTime);

	TArray<IMpSceneView*> SceneViews;
	TArray<IMpViewModel*> ViewModels;
};
