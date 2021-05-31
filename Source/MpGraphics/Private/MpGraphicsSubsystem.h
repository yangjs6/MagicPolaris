// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MpGraphicsSubsystem.generated.h"

UCLASS(BlueprintType, Transient)
class UMpGraphicsSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

	UMpGraphicsSubsystem();

public:
	// Begin USubsystem
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// End USubsystem
};