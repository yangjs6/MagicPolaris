// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IMpSceneView.h"
#include "MpEarthActor.generated.h"


class FMpViewCamera : public IMpViewCamera
{
public:
	//! 与地理坐标求交，视锥体拣选判断（-1：在视锥体外，0：与视锥体相交，1：在视锥体内）
	virtual int Intersets(const FGeoPosition& Pos) = 0;

	//! 视锥体与地理范围求交，包含返回正1，不包含返回-1，相交返回0
	virtual int Intersets(const FGeoBoundingBox& BBox) = 0;

	//! 得到观察的地理范围
	virtual void GetViewBoundingBox(FGeoBoundingBox& BBox) = 0;

	//! 获得相机地理位置
	virtual void GetCameraPosition(FGeoPosition& OutPos) = 0;

	//! 屏幕坐标转换为地理坐标	#屏幕坐标系定义:左下角为原点，x向右，y向上
	virtual void ComputePositionFromViewpoint(const FVector2D& InViewpoint, FGeoPosition& OutPosition) = 0;

	//! 地理坐标转换为屏幕坐标
	virtual void ComputeViewpointFromPosition(FVector2D& OutViewpoint, const FGeoPosition& InPosition) = 0;
};

UCLASS()
class AMpEarthActor : public AActor, public IMpSceneView
{
	GENERATED_BODY()
public:
	AMpEarthActor();

	virtual bool IsGameView();
	//! 获得UE4 世界
	virtual UWorld* IMpSceneView::GetWorld() const;
	//! 获得UE4 根组件
	virtual USceneComponent* GetRootComponent() const;

	virtual IMpViewCamera* GetCamera() const;

	virtual IMpEarthModel* GetEarthModel() const { return EarthModel.Get(); }

	virtual IMpTerrainModel* GetTerrainModel() const { return nullptr; }


private:
	TSharedPtr<IMpEarthModel> EarthModel;
};
