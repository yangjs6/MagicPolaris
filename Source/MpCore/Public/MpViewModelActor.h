// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/IMpViewModel.h"
#include "MpViewModelActor.generated.h"


UCLASS(abstract)
class MPCORE_API AMpViewModelActor : public AActor
{
	GENERATED_BODY()
public:
	virtual IMpViewModel* GetViewModel() PURE_VIRTUAL( , return nullptr;);
protected:

	virtual void PostRegisterAllComponents() override;
	virtual void PostUnregisterAllComponents() override;
};
