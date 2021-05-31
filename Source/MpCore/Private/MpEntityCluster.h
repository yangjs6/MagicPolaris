// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MpEntity.h"
#include "MpEntityCluster.generated.h"

class IMpSceneView;

/*

*/
UCLASS()
class UMpClusterEntity : public UMpEntity
{
	GENERATED_BODY()
public:
	void SetItems(const TArray<UMpEntity*>& InItems);

protected:
	TArray<UMpEntity*> Items;
};

UCLASS()
class UMpEntityCluster : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(Category = "Cluster", EditAnywhere, meta = (DisplayName = "enabled"))
		bool bEnabled;

	UPROPERTY(Category = "Cluster", EditAnywhere, BlueprintReadWrite, BlueprintSetter = SetPixelRange, meta = (EditCondition = "bEnabled", DisplayName = "pixelRange"))
		float PixelRange = 80;

	UPROPERTY(Category = "Cluster", EditAnywhere, BlueprintReadWrite, BlueprintSetter = SetMinimumClusterSize, meta = (EditCondition = "bEnabled", DisplayName = "minimumClusterSize"))
		int MinimumClusterSize = 2;
	
	UFUNCTION(BlueprintCallable, Category = "Cluster")
	void SetPixelRange(float InPixelRange);

	UFUNCTION(BlueprintCallable, Category = "Cluster")
	void SetMinimumClusterSize(int InMinimumClusterSize);

public:
	UMpEntityCluster();

	void ClusterEntities(IMpSceneView* SceneView, UMpEntityCollection* InEntityCollection);

	void MarkClusterDirty();

protected:

	bool bClusterDirty = false;

	UPROPERTY()
	UMpEntityCollection* ClusteredCollection;

	void ResetClusters();
	void AddCluster(const TArray<UMpEntity*>& Items);
	void AddNonClusteredItem(UMpEntity* Item);
};

