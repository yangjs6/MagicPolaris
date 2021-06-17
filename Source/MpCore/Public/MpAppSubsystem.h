// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Subsystems/WorldSubsystem.h"
#include "Engine/EngineTypes.h"
#include "Engine/Public/Tickable.h"
#include "Interfaces/IMpApp.h"
#include "MpViewModelManager.h"
#include "MpAppSubsystem.generated.h"

class IMpSceneView;
class IMpViewModel;
class AMpEarthActor;

/**
 * This is the API used to get core information at runtime
 */
UCLASS()
class UMpAppSubsystem : public UWorldSubsystem, public FTickableGameObject, public IMpApp
{
	GENERATED_BODY()

	UMpAppSubsystem();

public: // FTickableGameObject

	// FTickableGameObject implementation Begin
	virtual UWorld* GetTickableGameObjectWorld() const override { return GetWorld(); }
	virtual bool IsTickable() const override { return bInitialized; }
	virtual bool IsTickableInEditor() const override { return bInitialized; }
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;
	// FTickableGameObject implementation End

	// UWorldSubsystem implementation Begin
	/** Override to support water subsystems in editor preview worlds */
	virtual bool DoesSupportWorldType(EWorldType::Type WorldType) const override;
	// UWorldSubsystem implementation End

	// USubsystem implementation Begin
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// USubsystem implementation End

	/** Static helper function to get a water subsystem from a world, returns nullptr if world or subsystem don't exist */
	static UMpAppSubsystem* GetCoreSubsystem(const UWorld* InWorld);

	/** Static helper function to get a waterbody manager from a world, returns nullptr if world or manager don't exist */
	static FMpViewModelManager* GetViewModelManager(UWorld* InWorld);

	AMpEarthActor* GetEarthActor();
	FMpViewModelManager* GetViewModelManager();

public: // IMpApp
	//!绑定 场景视图
	virtual void AttachSceneView(IMpSceneView* SceneView);
	//!解除 场景视图
	virtual void DetachSceneView(IMpSceneView* SceneView);
	//!获得 场景视图 数量
	virtual int GetSceneViewNum();
	//!获得 场景视图
	virtual IMpSceneView* GetSceneView(int i);

	//!绑定 视图模型
	virtual void AttachViewModel(IMpViewModel* ViewModel);
	//!解除 视图模型
	virtual void DetachViewModel(IMpViewModel* ViewModel);
	//!获得 视图模型 数量
	virtual int GetViewModelNum();
	//!获得 视图模型
	virtual IMpViewModel* GetViewModel(int i);

protected:

private:
	UPROPERTY()
	mutable AMpEarthActor* EarthActor;
	FMpViewModelManager ViewModelManager;

	bool bInitialized;
};
