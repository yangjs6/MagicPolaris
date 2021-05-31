// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IMpSceneView.h"
#include "MpEarthActor.generated.h"


class FMpViewCamera : public IMpViewCamera
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

UCLASS()
class AMpEarthActor : public AActor, public IMpSceneView
{
	GENERATED_BODY()
public:
	AMpEarthActor();

	virtual bool IsGameView();
	//! ���UE4 ����
	virtual UWorld* IMpSceneView::GetWorld() const;
	//! ���UE4 �����
	virtual USceneComponent* GetRootComponent() const;

	virtual IMpViewCamera* GetCamera() const;

	virtual IMpEarthModel* GetEarthModel() const { return EarthModel.Get(); }

	virtual IMpTerrainModel* GetTerrainModel() const { return nullptr; }


private:
	TSharedPtr<IMpEarthModel> EarthModel;
};
