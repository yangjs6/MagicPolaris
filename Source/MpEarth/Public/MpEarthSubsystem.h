// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MpEarthSubsystem.generated.h"

class AMpEarthActor;
class IMpSceneView;

/**
 * This is the API used to get earth information at runtime
 */
UCLASS(BlueprintType, Transient)
class UMpEarthSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

	UMpEarthSubsystem();

public:
	// Begin USubsystem
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// End USubsystem

	AMpEarthActor* GetEarthActor();

protected:
	UPROPERTY()
	AMpEarthActor* EarthActor;
};