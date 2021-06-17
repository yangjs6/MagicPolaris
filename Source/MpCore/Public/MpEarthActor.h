// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/IMpSceneView.h"

#include "MpEarthCamera.h"
#include "MpEarthModel.h"

#include "MpEarthActor.generated.h"

UCLASS(Abstract)
class AMpEarthActor : public AActor, public IMpSceneView
{
	GENERATED_BODY()
public:
	AMpEarthActor();

	virtual bool IsGameView();

	virtual TScriptInterface<IMpViewCamera> GetCamera() const;

	virtual TScriptInterface<IMpEarthModel> GetEarthModel() const { return nullptr; }

	virtual TScriptInterface<IMpTerrainModel> GetTerrainModel() const { return nullptr; }


public:
	virtual void PostRegisterAllComponents() override;
	virtual void PostUnregisterAllComponents() override;

public:

	UPROPERTY(Category = "SceneView", VisibleAnywhere)
	UMpEarthCamera_Game* EarthCamera_Game;

private:
#if WITH_EDITOR
	UMpEarthCamera_Editor* EarthCamera_Editor;
#endif
};


UCLASS()
class AMpEarthActor_Ellipoid : public AMpEarthActor
{
	GENERATED_BODY()
public:
	AMpEarthActor_Ellipoid();

	virtual TScriptInterface<IMpEarthModel> GetEarthModel() const { return EarthModel; }

	UPROPERTY(Category = "EarthModel | Georeference", EditAnywhere)
		double OriginLongitude = 117.181559;

	UPROPERTY(Category = "EarthModel | Georeference", EditAnywhere)
		double OriginLatitude = 34.260993;

	UPROPERTY(Category = "EarthModel", EditAnywhere)
	UMpEarthModel_Ellipoid* EarthModel;

protected:

	virtual void OnConstruction(const FTransform& Transform) override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	void UpdateGeoreference();
};

UCLASS()
class AMpEarthActor_Projection : public AMpEarthActor
{
	GENERATED_BODY()
public:
	AMpEarthActor_Projection();

	virtual TScriptInterface<IMpEarthModel> GetEarthModel() const { return EarthModel; }

	UPROPERTY(Category = "EarthModel", EditAnywhere)
	FString SourceSRS = TEXT("+proj=longlat +ellps=WGS84 +datum=WGS84 +no_defs");

	UPROPERTY(Category = "EarthModel", EditAnywhere)
	FString TargetSRS = TEXT("+proj=ortho +lon_0=120 +lat_0=30 +a=6371010 +ellps=WGS84 +datum=WGS84 +no_defs");

	UPROPERTY(Category = "EarthModel", EditAnywhere)
	UMpEarthModel_Projection* EarthModel;


protected:
	virtual void OnConstruction(const FTransform& Transform) override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	void UpdateProjection();
};

