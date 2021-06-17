// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GeoEllipsoid.h"
#include "GeoProjection.h"
#include "Interfaces/IMpSceneView.h"

#include "MpEarthModel.generated.h"

//! 地球椭球模型
UCLASS()
class UMpEarthModel_Ellipoid : 
	public UObject,
	public IMpEarthModel
{
	GENERATED_BODY()
public:
	UMpEarthModel_Ellipoid();

	//! 将地理坐标转换为笛卡尔坐标
	virtual FGeoCartesian PositionToCartesian(const FGeoPosition& InPosition);

	//! 将笛卡尔坐标转换为地理坐标
	virtual FGeoPosition CartesianToPosition(const FGeoCartesian& InCartesian);

	//! 将UE坐标转换为笛卡尔坐标
	virtual FGeoCartesian UECoordinateToCartesian(const FVector& InCoordinate);

	//! 将笛卡尔坐标转换为UE坐标
	virtual FVector CartesianToUECoordinate(const FGeoCartesian& InCartesian);

	//! 将UE坐标转换为笛卡尔坐标
	virtual FGeoCartesian UECoordinateToCartesian(const FVector& InCoordinate, const FGeoFrame& LocalFrame);

	//! 将笛卡尔坐标转换为UE坐标
	virtual FVector CartesianToUECoordinate(const FGeoCartesian& InCartesian, const FGeoFrame& LocalFrame);

	//! 获得局地的坐标系
	virtual FGeoFrame ComputeFrameFromCartesian(const FGeoCartesian& InCartesian);

	//! 获得UE局地的变换
	virtual FTransform ComputeUETransformFromFrame(const FGeoFrame& InFrame);

	void UpdateFrame(const FGeoPosition& InPosition);

protected:
	FGeoEllipsoid Ellipoid;
	FGeoFrame RefFrame;
};



//! 地球投影模型
UCLASS()
class UMpEarthModel_Projection :
	public UObject,
	public IMpEarthModel
{
	GENERATED_BODY()
public:
	UMpEarthModel_Projection();

	//! 将地理坐标转换为笛卡尔坐标
	virtual FGeoCartesian PositionToCartesian(const FGeoPosition& InPosition);

	//! 将笛卡尔坐标转换为地理坐标
	virtual FGeoPosition CartesianToPosition(const FGeoCartesian& InCartesian);

	//! 将UE坐标转换为笛卡尔坐标
	virtual FGeoCartesian UECoordinateToCartesian(const FVector& InCoordinate);

	//! 将笛卡尔坐标转换为UE坐标
	virtual FVector CartesianToUECoordinate(const FGeoCartesian& InCartesian);

	//! 将UE坐标转换为笛卡尔坐标
	virtual FGeoCartesian UECoordinateToCartesian(const FVector& InCoordinate, const FGeoFrame& LocalFrame);

	//! 将笛卡尔坐标转换为UE坐标
	virtual FVector CartesianToUECoordinate(const FGeoCartesian& InCartesian, const FGeoFrame& LocalFrame);

	//! 获得局地的坐标系
	virtual FGeoFrame ComputeFrameFromCartesian(const FGeoCartesian& InCartesian);

	//! 获得UE局地的变换
	virtual FTransform ComputeUETransformFromFrame(const FGeoFrame& InFrame);

public:
	void SetProjection(const FString& SourceSRS, const FString& TargetSRS);

	void SetOffset(const FVector& InOffset) { Offset = InOffset; }
protected:
	FVector Offset;
	FGeoProjection Projection;
};