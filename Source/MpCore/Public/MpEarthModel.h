// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GeoEllipsoid.h"
#include "GeoProjection.h"
#include "Interfaces/IMpSceneView.h"

#include "MpEarthModel.generated.h"

//! ��������ģ��
UCLASS()
class UMpEarthModel_Ellipoid : 
	public UObject,
	public IMpEarthModel
{
	GENERATED_BODY()
public:
	UMpEarthModel_Ellipoid();

	//! ����������ת��Ϊ�ѿ�������
	virtual FGeoCartesian PositionToCartesian(const FGeoPosition& InPosition);

	//! ���ѿ�������ת��Ϊ��������
	virtual FGeoPosition CartesianToPosition(const FGeoCartesian& InCartesian);

	//! ��UE����ת��Ϊ�ѿ�������
	virtual FGeoCartesian UECoordinateToCartesian(const FVector& InCoordinate);

	//! ���ѿ�������ת��ΪUE����
	virtual FVector CartesianToUECoordinate(const FGeoCartesian& InCartesian);

	//! ��UE����ת��Ϊ�ѿ�������
	virtual FGeoCartesian UECoordinateToCartesian(const FVector& InCoordinate, const FGeoFrame& LocalFrame);

	//! ���ѿ�������ת��ΪUE����
	virtual FVector CartesianToUECoordinate(const FGeoCartesian& InCartesian, const FGeoFrame& LocalFrame);

	//! ��þֵص�����ϵ
	virtual FGeoFrame ComputeFrameFromCartesian(const FGeoCartesian& InCartesian);

	//! ���UE�ֵصı任
	virtual FTransform ComputeUETransformFromFrame(const FGeoFrame& InFrame);

	void UpdateFrame(const FGeoPosition& InPosition);

protected:
	FGeoEllipsoid Ellipoid;
	FGeoFrame RefFrame;
};



//! ����ͶӰģ��
UCLASS()
class UMpEarthModel_Projection :
	public UObject,
	public IMpEarthModel
{
	GENERATED_BODY()
public:
	UMpEarthModel_Projection();

	//! ����������ת��Ϊ�ѿ�������
	virtual FGeoCartesian PositionToCartesian(const FGeoPosition& InPosition);

	//! ���ѿ�������ת��Ϊ��������
	virtual FGeoPosition CartesianToPosition(const FGeoCartesian& InCartesian);

	//! ��UE����ת��Ϊ�ѿ�������
	virtual FGeoCartesian UECoordinateToCartesian(const FVector& InCoordinate);

	//! ���ѿ�������ת��ΪUE����
	virtual FVector CartesianToUECoordinate(const FGeoCartesian& InCartesian);

	//! ��UE����ת��Ϊ�ѿ�������
	virtual FGeoCartesian UECoordinateToCartesian(const FVector& InCoordinate, const FGeoFrame& LocalFrame);

	//! ���ѿ�������ת��ΪUE����
	virtual FVector CartesianToUECoordinate(const FGeoCartesian& InCartesian, const FGeoFrame& LocalFrame);

	//! ��þֵص�����ϵ
	virtual FGeoFrame ComputeFrameFromCartesian(const FGeoCartesian& InCartesian);

	//! ���UE�ֵصı任
	virtual FTransform ComputeUETransformFromFrame(const FGeoFrame& InFrame);

public:
	void SetProjection(const FString& SourceSRS, const FString& TargetSRS);

	void SetOffset(const FVector& InOffset) { Offset = InOffset; }
protected:
	FVector Offset;
	FGeoProjection Projection;
};