// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Interfaces/IMpSceneView.h"
#include "MpEarthCamera.generated.h"

UCLASS()
class UMpEarthCamera_Game:
	public UCameraComponent,
	public IMpViewCamera
{
	GENERATED_BODY()
public:
	UMpEarthCamera_Game();


	//! ����������󽻣���׶���ѡ�жϣ�-1������׶���⣬0������׶���ཻ��1������׶���ڣ�
	virtual int Intersets(const FGeoPosition& Pos) { return 1; }

	//! ��׶�������Χ�󽻣�����������1������������-1���ཻ����0
	virtual int Intersets(const FGeoBoundingBox& BBox) { return 1; }

protected:
};

#if WITH_EDITOR

UCLASS()
class UMpEarthCamera_Editor : 
	public UObject,
	public IMpViewCamera
{
	GENERATED_BODY()
public:
	UMpEarthCamera_Editor();


	//! ����������󽻣���׶���ѡ�жϣ�-1������׶���⣬0������׶���ཻ��1������׶���ڣ�
	virtual int Intersets(const FGeoPosition& Pos) { return 1; }

	//! ��׶�������Χ�󽻣�����������1������������-1���ཻ����0
	virtual int Intersets(const FGeoBoundingBox& BBox) { return 1; }

};

#endif