// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MpTypes.h"

class UWorld;
class USceneComponent;


//! 地球数据模型
class IMpEarthModel
{
public:
	//! 将地理坐标转换为笛卡尔坐标
	virtual FGeoCartesian PositionToCartesian(const FGeoPosition& InPosition) = 0;

	//! 将笛卡尔坐标转换为地理坐标
	virtual FGeoPosition CartesianToPosition(const FGeoCartesian& InCartesian) = 0;

	//! 将UE坐标转换为笛卡尔坐标
	virtual FGeoCartesian UECoordinateToCartesian(const FVector& InCoordinate) = 0;

	//! 将笛卡尔坐标转换为UE坐标
	virtual FVector CartesianToUECoordinate(const FGeoCartesian& InCartesian) = 0;

	//! 将地理坐标转换为UE坐标
	virtual FVector PositionToUECoordinate(const FGeoPosition& InPosition) = 0;

	//! 将UE坐标转换为地理坐标
	virtual FGeoPosition UECoordinateToPosition(const FVector& InCoordinate) = 0;

	//! 获得局地的坐标系
	virtual FGeoFrame ComputeFrameFromPosition(const FGeoPosition& InPosition) = 0;

	//! 获得局地的坐标系
	virtual FGeoFrame ComputeFrameFromCartesian(const FGeoCartesian& InCartesian) = 0;
};

class IMpTerrainModel
{
public:
	//! 锁定地形区域
	virtual bool LockTerrainRegion(const FGeoBoundingBox& BoundingBox, double cellResolution = 1) = 0;

	//! 解除锁定
	virtual void UnlockTerrainRegion() = 0;

	//! 获得高程
	virtual bool GetTerrainHeight(FGeoPosition& InPosition) = 0;
};

class IMpViewCamera
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

class IMpSceneView
{
public:
	//! 是否是游戏视图还是编辑器视图
	virtual bool IsGameView() = 0;
	//! 获得UE4 世界
	virtual UWorld* GetWorld() const = 0;
	//! 获得UE4 根组件
	virtual USceneComponent* GetRootComponent() const = 0;

	//! 获得相机
	virtual IMpViewCamera* GetCamera() const = 0;

	//! 获得地球模型
	virtual IMpEarthModel* GetEarthModel() const = 0;

	//! 获得地形模型
	virtual IMpTerrainModel* GetTerrainModel() const = 0;
};
