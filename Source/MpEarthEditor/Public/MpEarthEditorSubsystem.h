// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "IMpSceneView.h"
#include "EditorSubsystem.h"
#include "MpEarthEditorSubsystem.generated.h"

class UMpEditorSceneView;
//UCLASS()
//class UMpEditorSceneView : public UObject, public IMpSceneView
//{
//	GENERATED_BODY()
//public:
//
//	virtual bool IsGameView() { return false; }
//
//
//	virtual IMpViewCamera* GetCamera() { return nullptr; }
//
//	virtual IMpEarthModel* GetEarthModel() { return nullptr; }
//
//	virtual IMpTerrainModel* GetTerrainModel() { return nullptr; }
//};

/**
 * This is the API used to get earth information at runtime
 */
UCLASS(BlueprintType, Transient)
class UMpEarthEditorSubsystem : public UEditorSubsystem
{
	GENERATED_BODY()

	UMpEarthEditorSubsystem();

public:
	// Begin USubsystem
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// End USubsystem

protected:
	//UPROPERTY()
	//UMpEditorSceneView* SceneView;
};
