// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UObject/ScriptInterface.h"
#include "MpTypes.h"
#include "IMpSceneView.generated.h"

UINTERFACE()
class MPCORE_API UMpEarthModel : public UInterface
{
public:
	GENERATED_BODY()
};

//! 地球数据模型
class IMpEarthModel : public IInterface
{
	GENERATED_BODY()
public:

	//! 将地理坐标转换为笛卡尔坐标
	virtual FGeoCartesian PositionToCartesian(const FGeoPosition& InPosition) = 0;

	//! 将笛卡尔坐标转换为地理坐标
	virtual FGeoPosition CartesianToPosition(const FGeoCartesian& InCartesian) = 0;

	//! 将UE坐标转换为笛卡尔坐标
	virtual FGeoCartesian UECoordinateToCartesian(const FVector& InCoordinate) = 0;

	//! 将笛卡尔坐标转换为UE坐标
	virtual FVector CartesianToUECoordinate(const FGeoCartesian& InCartesian) = 0;

	//! 将UE坐标转换为笛卡尔坐标
	virtual FGeoCartesian UECoordinateToCartesian(const FVector& InCoordinate, const FGeoFrame& LocalFrame) = 0;

	//! 将笛卡尔坐标转换为UE坐标
	virtual FVector CartesianToUECoordinate(const FGeoCartesian& InCartesian, const FGeoFrame& LocalFrame) = 0;

	//! 将地理坐标转换为UE坐标
	virtual FVector PositionToUECoordinate(const FGeoPosition& InPosition)
	{
		FGeoCartesian InCartesian = PositionToCartesian(InPosition);
		return CartesianToUECoordinate(InCartesian);
	}

	//! 将UE坐标转换为地理坐标
	virtual FGeoPosition UECoordinateToPosition(const FVector& InCoordinate, const FGeoFrame& LocalFrame)
	{
		FGeoCartesian InCartesian = UECoordinateToCartesian(InCoordinate, LocalFrame);
		return CartesianToPosition(InCartesian);
	}

	//! 将地理坐标转换为UE坐标
	virtual FVector PositionToUECoordinate(const FGeoPosition& InPosition, const FGeoFrame& LocalFrame)
	{
		FGeoCartesian InCartesian = PositionToCartesian(InPosition);
		return CartesianToUECoordinate(InCartesian, LocalFrame);
	}

	//! 将UE坐标转换为地理坐标
	virtual FGeoPosition UECoordinateToPosition(const FVector& InCoordinate)
	{
		FGeoCartesian InCartesian = UECoordinateToCartesian(InCoordinate);
		return CartesianToPosition(InCartesian);
	}

	//! 获得局地的坐标系
	virtual FGeoFrame ComputeFrameFromPosition(const FGeoPosition& InPosition)
	{
		FGeoCartesian InCartesian = PositionToCartesian(InPosition);
		return ComputeFrameFromCartesian(InCartesian);
	}

	//! 获得局地的坐标系
	virtual FGeoFrame ComputeFrameFromCartesian(const FGeoCartesian& InCartesian) = 0;

	//! 获得UE局地的变换
	virtual FTransform ComputeUETransformFromFrame(const FGeoFrame& InFrame) = 0;
};

UINTERFACE()
class MPCORE_API UMpTerrainModel : public UInterface
{
public:
	GENERATED_BODY()
};

//! 地形模型
class IMpTerrainModel : public IInterface
{
	GENERATED_BODY()
public:
	//! 锁定地形区域
	virtual bool LockTerrainRegion(const FGeoBoundingBox& BoundingBox, double cellResolution = 1) = 0;

	//! 解除锁定
	virtual void UnlockTerrainRegion() = 0;

	//! 获得高程
	virtual bool GetTerrainHeight(FGeoPosition& InPosition) = 0;
};

UINTERFACE()
class MPCORE_API UMpViewCamera : public UInterface
{
public:
	GENERATED_BODY()
};

//! 视图相机
class IMpViewCamera : public IInterface
{
	GENERATED_BODY()
public:
	//! 与地理坐标求交，视锥体拣选判断（-1：在视锥体外，0：与视锥体相交，1：在视锥体内）
	virtual int Intersets(const FGeoPosition& Pos) = 0;

	//! 视锥体与地理范围求交，包含返回正1，不包含返回-1，相交返回0
	virtual int Intersets(const FGeoBoundingBox& BBox) = 0;

	////! 得到观察的地理范围
	//virtual void GetViewBoundingBox(FGeoBoundingBox& OutBBox) = 0;

	////! 获得相机地理位置
	//virtual void GetCameraPosition(FGeoPosition& OutPos) = 0;

	////! 屏幕坐标转换为地理坐标	#屏幕坐标系定义:左下角为原点，x向右，y向上
	//virtual void ComputePositionFromViewpoint(const FVector2D& InViewpoint, FGeoPosition& OutPosition) = 0;

	////! 地理坐标转换为屏幕坐标
	//virtual void ComputeViewpointFromPosition(FVector2D& OutViewpoint, const FGeoPosition& InPosition) = 0;
};


UINTERFACE()
class MPCORE_API UMpSceneView : public UInterface
{
public:
	GENERATED_BODY()
};

//! 场景视图
class IMpSceneView : public IInterface
{
	GENERATED_BODY()
public:
	//! 是否是游戏视图还是编辑器视图
	virtual bool IsGameView() = 0;

	//! 获得相机
	virtual TScriptInterface<IMpViewCamera> GetCamera() const = 0;

	//! 获得地球模型
	virtual TScriptInterface<IMpEarthModel> GetEarthModel() const = 0;

	//! 获得地形模型
	virtual TScriptInterface<IMpTerrainModel> GetTerrainModel() const = 0;
};
