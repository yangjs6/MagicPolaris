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


	//! 与地理坐标求交，视锥体拣选判断（-1：在视锥体外，0：与视锥体相交，1：在视锥体内）
	virtual int Intersets(const FGeoPosition& Pos) { return 1; }

	//! 视锥体与地理范围求交，包含返回正1，不包含返回-1，相交返回0
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


	//! 与地理坐标求交，视锥体拣选判断（-1：在视锥体外，0：与视锥体相交，1：在视锥体内）
	virtual int Intersets(const FGeoPosition& Pos) { return 1; }

	//! 视锥体与地理范围求交，包含返回正1，不包含返回-1，相交返回0
	virtual int Intersets(const FGeoBoundingBox& BBox) { return 1; }

};

#endif