// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GeoPosition.h"

#include "MpTypes.generated.h"

UENUM()
enum class EHorizontalOrigin : int8
{
	Invalid = -1 UMETA(Hidden),

	/** The origin is on the left side of the object.*/
	LEFT = 0,

	/** The origin is at the horizontal center of the object. */
	CENTER = 1,

	/** The origin is on the right side of the object. */
	RIGHT = 2,
};

UENUM()
enum class EVerticalOrigin : int8
{
	Invalid = -1 UMETA(Hidden),

	/** If the object contains text, the origin is at the baseline of the text, otherwise the origin is at the bottom of the object.*/
	BASELINE = 0,

	/** The origin is at the bottom of the object. */
	BOTTOM = 1,

	/** The origin is at the vertical center between `BASELINE` and `TOP`.*/
	CENTER = 2,

	/** The origin is at the top of the object. */
	TOP = 2,
};

UENUM()
enum class EHeightReference : int8
{
	Invalid = -1 UMETA(Hidden),

	/** The position is absolute.*/
	NONE = 0,

	/** The position is clamped to the terrain.*/
	CLAMP_TO_GROUND = 1,

	/** The position height is the height above the terrain.*/
	RELATIVE_TO_GROUND = 2,
};

USTRUCT()
struct FNearFarScalar
{
	GENERATED_BODY()

	UPROPERTY()
	float NearDistance;

	UPROPERTY()
	float NearValue;

	UPROPERTY()
	float FarDistance;

	UPROPERTY()
	float FarValue;
};


