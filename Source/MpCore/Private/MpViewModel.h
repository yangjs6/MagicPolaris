// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MpViewModel.generated.h"


UCLASS()
class UMpViewModel : public UObject
{
	GENERATED_BODY()
public:
	void AddEntity(UMpEntity* Entity);

protected:
	UPROPERTY()
	TArray<UMpEntity*> Entities;
};
