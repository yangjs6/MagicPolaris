// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GeoEllipsoid.h"
#include "IMpSceneView.h"

//! 地球数据模型
class FMpEarthModel_Ellipoid : public IMpEarthModel
{
public:
	FMpEarthModel_Ellipoid(const FGeoPosition& InGeoreference = FGeoPosition());

	FMpEarthModel_Ellipoid(double a, double b, const FGeoPosition& InGeoreference = FGeoPosition());

	//! 将地理坐标转换为笛卡尔坐标
	virtual FGeoCartesian PositionToCartesian(const FGeoPosition& InPosition);

	//! 将笛卡尔坐标转换为地理坐标
	virtual FGeoPosition CartesianToPosition(const FGeoCartesian& InCartesian);

	//! 将UE坐标转换为笛卡尔坐标
	virtual FGeoCartesian UECoordinateToCartesian(const FVector& InCoordinate);

	//! 将笛卡尔坐标转换为UE坐标
	virtual FVector CartesianToUECoordinate(const FGeoCartesian& InCartesian);

	//! 将地理坐标转换为UE坐标
	virtual FVector PositionToUECoordinate(const FGeoPosition& InPosition);

	//! 将UE坐标转换为地理坐标
	virtual FGeoPosition UECoordinateToPosition(const FVector& InCoordinate);

	//! 获得局地的坐标系
	virtual FGeoFrame ComputeFrameFromPosition(const FGeoPosition& InPosition);

	//! 获得局地的坐标系
	virtual FGeoFrame ComputeFrameFromCartesian(const FGeoCartesian& InCartesian);

public:
	void SetGeoreference(const FGeoPosition& InGeoreference);

	const FGeoPosition& GetGeoreference() { return Georeference; }


protected:
	FGeoEllipsoid Ellipoid;

	FGeoPosition Georeference;
	FGeoFrame RefFrame;
};
