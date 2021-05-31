// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "IMpSceneView.h"
#include "MpViewSubsystem.generated.h"

//UCLASS()
//class UMpLocalPlayerSceneView : public UObject, public IMpSceneView
//{
//	GENERATED_BODY()
//public:
//
//	virtual bool IsGameView() { return true; }
//
//	virtual IMpViewCamera* GetCamera() { return nullptr; }
//
//	virtual IMpEarthModel* GetEarthModel() { return nullptr; }
//
//	virtual IMpTerrainModel* GetTerrainModel() { return nullptr; }
//
//	void AttachLocalPlayer(ULocalPlayer* InLocalPlayer);
//	void DetachLocalPlayer();
//
//protected:
//
//	TWeakObjectPtr<ULocalPlayer> LocalPlayer;
//};

/**
 * This is the API used to get earth information at runtime
 */
UCLASS(BlueprintType, Transient)
class MPCORE_API UMpViewSubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()

	UMpViewSubsystem();

public:
	// Begin USubsystem
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// End USubsystem

protected:
	//UPROPERTY()
	//UMpLocalPlayerSceneView* SceneView;
};
