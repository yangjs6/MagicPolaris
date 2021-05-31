// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MpTypes.h"

class UWorld;
class USceneComponent;


//! ��������ģ��
class IMpEarthModel
{
public:
	//! ����������ת��Ϊ�ѿ�������
	virtual FGeoCartesian PositionToCartesian(const FGeoPosition& InPosition) = 0;

	//! ���ѿ�������ת��Ϊ��������
	virtual FGeoPosition CartesianToPosition(const FGeoCartesian& InCartesian) = 0;

	//! ��UE����ת��Ϊ�ѿ�������
	virtual FGeoCartesian UECoordinateToCartesian(const FVector& InCoordinate) = 0;

	//! ���ѿ�������ת��ΪUE����
	virtual FVector CartesianToUECoordinate(const FGeoCartesian& InCartesian) = 0;

	//! ����������ת��ΪUE����
	virtual FVector PositionToUECoordinate(const FGeoPosition& InPosition) = 0;

	//! ��UE����ת��Ϊ��������
	virtual FGeoPosition UECoordinateToPosition(const FVector& InCoordinate) = 0;

	//! ��þֵص�����ϵ
	virtual FGeoFrame ComputeFrameFromPosition(const FGeoPosition& InPosition) = 0;

	//! ��þֵص�����ϵ
	virtual FGeoFrame ComputeFrameFromCartesian(const FGeoCartesian& InCartesian) = 0;
};

class IMpTerrainModel
{
public:
	//! ������������
	virtual bool LockTerrainRegion(const FGeoBoundingBox& BoundingBox, double cellResolution = 1) = 0;

	//! �������
	virtual void UnlockTerrainRegion() = 0;

	//! ��ø߳�
	virtual bool GetTerrainHeight(FGeoPosition& InPosition) = 0;
};

class IMpViewCamera
{
public:
	//! ����������󽻣���׶���ѡ�жϣ�-1������׶���⣬0������׶���ཻ��1������׶���ڣ�
	virtual int Intersets(const FGeoPosition& Pos) = 0;

	//! ��׶�������Χ�󽻣�����������1������������-1���ཻ����0
	virtual int Intersets(const FGeoBoundingBox& BBox) = 0;

	//! �õ��۲�ĵ���Χ
	virtual void GetViewBoundingBox(FGeoBoundingBox& BBox) = 0;

	//! ����������λ��
	virtual void GetCameraPosition(FGeoPosition& OutPos) = 0;

	//! ��Ļ����ת��Ϊ��������	#��Ļ����ϵ����:���½�Ϊԭ�㣬x���ң�y����
	virtual void ComputePositionFromViewpoint(const FVector2D& InViewpoint, FGeoPosition& OutPosition) = 0;

	//! ��������ת��Ϊ��Ļ����
	virtual void ComputeViewpointFromPosition(FVector2D& OutViewpoint, const FGeoPosition& InPosition) = 0;
};

class IMpSceneView
{
public:
	//! �Ƿ�����Ϸ��ͼ���Ǳ༭����ͼ
	virtual bool IsGameView() = 0;
	//! ���UE4 ����
	virtual UWorld* GetWorld() const = 0;
	//! ���UE4 �����
	virtual USceneComponent* GetRootComponent() const = 0;

	//! ������
	virtual IMpViewCamera* GetCamera() const = 0;

	//! ��õ���ģ��
	virtual IMpEarthModel* GetEarthModel() const = 0;

	//! ��õ���ģ��
	virtual IMpTerrainModel* GetTerrainModel() const = 0;
};
