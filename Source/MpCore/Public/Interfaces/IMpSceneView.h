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

//! ��������ģ��
class IMpEarthModel : public IInterface
{
	GENERATED_BODY()
public:

	//! ����������ת��Ϊ�ѿ�������
	virtual FGeoCartesian PositionToCartesian(const FGeoPosition& InPosition) = 0;

	//! ���ѿ�������ת��Ϊ��������
	virtual FGeoPosition CartesianToPosition(const FGeoCartesian& InCartesian) = 0;

	//! ��UE����ת��Ϊ�ѿ�������
	virtual FGeoCartesian UECoordinateToCartesian(const FVector& InCoordinate) = 0;

	//! ���ѿ�������ת��ΪUE����
	virtual FVector CartesianToUECoordinate(const FGeoCartesian& InCartesian) = 0;

	//! ��UE����ת��Ϊ�ѿ�������
	virtual FGeoCartesian UECoordinateToCartesian(const FVector& InCoordinate, const FGeoFrame& LocalFrame) = 0;

	//! ���ѿ�������ת��ΪUE����
	virtual FVector CartesianToUECoordinate(const FGeoCartesian& InCartesian, const FGeoFrame& LocalFrame) = 0;

	//! ����������ת��ΪUE����
	virtual FVector PositionToUECoordinate(const FGeoPosition& InPosition)
	{
		FGeoCartesian InCartesian = PositionToCartesian(InPosition);
		return CartesianToUECoordinate(InCartesian);
	}

	//! ��UE����ת��Ϊ��������
	virtual FGeoPosition UECoordinateToPosition(const FVector& InCoordinate, const FGeoFrame& LocalFrame)
	{
		FGeoCartesian InCartesian = UECoordinateToCartesian(InCoordinate, LocalFrame);
		return CartesianToPosition(InCartesian);
	}

	//! ����������ת��ΪUE����
	virtual FVector PositionToUECoordinate(const FGeoPosition& InPosition, const FGeoFrame& LocalFrame)
	{
		FGeoCartesian InCartesian = PositionToCartesian(InPosition);
		return CartesianToUECoordinate(InCartesian, LocalFrame);
	}

	//! ��UE����ת��Ϊ��������
	virtual FGeoPosition UECoordinateToPosition(const FVector& InCoordinate)
	{
		FGeoCartesian InCartesian = UECoordinateToCartesian(InCoordinate);
		return CartesianToPosition(InCartesian);
	}

	//! ��þֵص�����ϵ
	virtual FGeoFrame ComputeFrameFromPosition(const FGeoPosition& InPosition)
	{
		FGeoCartesian InCartesian = PositionToCartesian(InPosition);
		return ComputeFrameFromCartesian(InCartesian);
	}

	//! ��þֵص�����ϵ
	virtual FGeoFrame ComputeFrameFromCartesian(const FGeoCartesian& InCartesian) = 0;

	//! ���UE�ֵصı任
	virtual FTransform ComputeUETransformFromFrame(const FGeoFrame& InFrame) = 0;
};

UINTERFACE()
class MPCORE_API UMpTerrainModel : public UInterface
{
public:
	GENERATED_BODY()
};

//! ����ģ��
class IMpTerrainModel : public IInterface
{
	GENERATED_BODY()
public:
	//! ������������
	virtual bool LockTerrainRegion(const FGeoBoundingBox& BoundingBox, double cellResolution = 1) = 0;

	//! �������
	virtual void UnlockTerrainRegion() = 0;

	//! ��ø߳�
	virtual bool GetTerrainHeight(FGeoPosition& InPosition) = 0;
};

UINTERFACE()
class MPCORE_API UMpViewCamera : public UInterface
{
public:
	GENERATED_BODY()
};

//! ��ͼ���
class IMpViewCamera : public IInterface
{
	GENERATED_BODY()
public:
	//! ����������󽻣���׶���ѡ�жϣ�-1������׶���⣬0������׶���ཻ��1������׶���ڣ�
	virtual int Intersets(const FGeoPosition& Pos) = 0;

	//! ��׶�������Χ�󽻣�����������1������������-1���ཻ����0
	virtual int Intersets(const FGeoBoundingBox& BBox) = 0;

	////! �õ��۲�ĵ���Χ
	//virtual void GetViewBoundingBox(FGeoBoundingBox& OutBBox) = 0;

	////! ����������λ��
	//virtual void GetCameraPosition(FGeoPosition& OutPos) = 0;

	////! ��Ļ����ת��Ϊ��������	#��Ļ����ϵ����:���½�Ϊԭ�㣬x���ң�y����
	//virtual void ComputePositionFromViewpoint(const FVector2D& InViewpoint, FGeoPosition& OutPosition) = 0;

	////! ��������ת��Ϊ��Ļ����
	//virtual void ComputeViewpointFromPosition(FVector2D& OutViewpoint, const FGeoPosition& InPosition) = 0;
};


UINTERFACE()
class MPCORE_API UMpSceneView : public UInterface
{
public:
	GENERATED_BODY()
};

//! ������ͼ
class IMpSceneView : public IInterface
{
	GENERATED_BODY()
public:
	//! �Ƿ�����Ϸ��ͼ���Ǳ༭����ͼ
	virtual bool IsGameView() = 0;

	//! ������
	virtual TScriptInterface<IMpViewCamera> GetCamera() const = 0;

	//! ��õ���ģ��
	virtual TScriptInterface<IMpEarthModel> GetEarthModel() const = 0;

	//! ��õ���ģ��
	virtual TScriptInterface<IMpTerrainModel> GetTerrainModel() const = 0;
};
