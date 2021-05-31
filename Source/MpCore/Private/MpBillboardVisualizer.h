// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MpVisualizer.h"
#include "MpBillboardVisualizer.generated.h"

class UMpEntityCollection;
class UMpEntityCluster;

UCLASS()
class UMpBillboardVisualizer : public UMpVisualizer
{
	GENERATED_BODY()
public:
	void Initialize(UMpEntityCollection* InEntityCollection, UMpEntityCluster* InEntityCluster);
	
	virtual void UpdateEntities(float Time);

	virtual void UpdateView(IMpSceneView* SceneView);

protected:

	UPROPERTY()
	UMpEntityCollection* EntityCollection;

	UPROPERTY()
	UMpEntityCluster* EntityCluster;
};
