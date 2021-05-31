// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GeoEllipsoid.h"
#include "IMpSceneView.h"

//! ��������ģ��
class FMpEarthModel_Ellipoid : public IMpEarthModel
{
public:
	FMpEarthModel_Ellipoid(const FGeoPosition& InGeoreference = FGeoPosition());

	FMpEarthModel_Ellipoid(double a, double b, const FGeoPosition& InGeoreference = FGeoPosition());

	//! ����������ת��Ϊ�ѿ�������
	virtual FGeoCartesian PositionToCartesian(const FGeoPosition& InPosition);

	//! ���ѿ�������ת��Ϊ��������
	virtual FGeoPosition CartesianToPosition(const FGeoCartesian& InCartesian);

	//! ��UE����ת��Ϊ�ѿ�������
	virtual FGeoCartesian UECoordinateToCartesian(const FVector& InCoordinate);

	//! ���ѿ�������ת��ΪUE����
	virtual FVector CartesianToUECoordinate(const FGeoCartesian& InCartesian);

	//! ����������ת��ΪUE����
	virtual FVector PositionToUECoordinate(const FGeoPosition& InPosition);

	//! ��UE����ת��Ϊ��������
	virtual FGeoPosition UECoordinateToPosition(const FVector& InCoordinate);

	//! ��þֵص�����ϵ
	virtual FGeoFrame ComputeFrameFromPosition(const FGeoPosition& InPosition);

	//! ��þֵص�����ϵ
	virtual FGeoFrame ComputeFrameFromCartesian(const FGeoCartesian& InCartesian);

public:
	void SetGeoreference(const FGeoPosition& InGeoreference);

	const FGeoPosition& GetGeoreference() { return Georeference; }


protected:
	FGeoEllipsoid Ellipoid;

	FGeoPosition Georeference;
	FGeoFrame RefFrame;
};
