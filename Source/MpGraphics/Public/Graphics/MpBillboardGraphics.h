// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MpGraphics.h"
#include "MpBillboardGraphics.generated.h"

UCLASS()
class UMpBillboardGraphics : public UMpGraphics
{
	GENERATED_BODY()
public:

	// The URI of the image displayed on the billboard. For broadest client compatibility
	UPROPERTY(Category = "Graphics", EditAnywhere, meta = (DisplayName = "image"))
	FString Image;

	// The scale of the billboard. The scale is multiplied with the pixel size of the billboard's `image`.
	// For example, if the scale is 2.0, the billboard will be rendered with twice the number of pixels,
	// in each direction, of the `image`.
	UPROPERTY(Category = "Graphics", EditAnywhere, meta = (DisplayName = "scale"))
	float Scale = 1.0;

	// The offset, in viewport pixels, of the billboard origin from the `position`. 
	// A pixel offset is the number of pixels up and to the right to place the billboard, relative to the `position`.
	UPROPERTY(Category = "Graphics", EditAnywhere, meta = (DisplayName = "pixelOffset"))
	FVector2D PixelOffset = FVector2D(0.0, 0.0);

	// The eye offset of the billboard, which is the offset in eye coordinates at which to place the billboard 
	// relative to the `position` property. Eye coordinates are a left-handed coordinate system where the X-axis
	// points toward the viewer's right, the Y-axis points up, and the Z-axis points into the screen.
	UPROPERTY(Category = "Graphics", EditAnywhere, meta = (DisplayName = "eyeOffset"))
	FVector EyeOffset = FVector(0.0, 0.0, 0.0);

	// The horizontal origin of the billboard, which determines whether the billboard image is left-, center-, 
	// or right-aligned with the `position`..
	UPROPERTY(Category = "Graphics", EditAnywhere, meta = (DisplayName = "horizontalOrigin"))
	EHorizontalOrigin HorizontalOrigin = EHorizontalOrigin::CENTER;

	// The vertical origin of the billboard, which determines whether the billboard image is bottom-, center-,
	// or top-aligned with the `position`..
	UPROPERTY(Category = "Graphics", EditAnywhere, meta = (DisplayName = "verticalOrigin"))
	EVerticalOrigin VerticalOrigin = EVerticalOrigin::CENTER;

	// The height reference of the billboard, which indicates if the position is relative to terrain or not.
	UPROPERTY(Category = "Graphics", EditAnywhere, meta = (DisplayName = "heightReference"))
	EHeightReference HeightReference = EHeightReference::NONE;

	// The color of the billboard. This color value is multiplied with the values of the billboard's `image` to produce the final color.
	UPROPERTY(Category = "Graphics", EditAnywhere, meta = (DisplayName = "color"))
	FColor Color = FColor::White;

	// The rotation of the billboard, in radians, counter-clockwise from the alignedAxis..
	UPROPERTY(Category = "Graphics", EditAnywhere, meta = (DisplayName = "rotation"))
	float Rotation = 0.0;

	// The aligned axis is the unit vector, in world coordinates, that the billboard up vector points towards. 
	// The default is the zero vector, which means the billboard is aligned to the screen up vector.
	UPROPERTY(Category = "Graphics", EditAnywhere, meta = (DisplayName = "alignedAxis"))
	FVector alignedAxis = FVector(0.0, 0.0, 0.0);

	// Whether this billboard's size (`width` and `height`) should be measured in meters, 
	// otherwise size is measured in pixels.
	UPROPERTY(Category = "Graphics", EditAnywhere, meta = (DisplayName = "sizeInMeters"))
	bool SizeInMeters = false;

	// The width of the billboard, in pixels (or meters, if `sizeInMeters` is true).
	// By default, the native width of the image is used.
	UPROPERTY(Category = "Graphics", EditAnywhere, meta = (DisplayName = "width"))
	float Width = 0.0;

	// The height of the billboard, in pixels (or meters, if `sizeInMeters` is true).
	// By default, the native height of the image is used.
	UPROPERTY(Category = "Graphics", EditAnywhere, meta = (DisplayName = "height"))
	float Height = 0.0;

	// How the billboard's scale should change based on the billboard's distance from the camera. This scalar value will be multiplied by `scale`.
	UPROPERTY(Category = "Graphics", EditAnywhere, meta = (DisplayName = "scaleByDistance"))
	FNearFarScalar ScaleByDistance;

	// How the billboard's translucency should change based on the billboard's distance from the camera. This scalar value should range from 0 to 1.
	UPROPERTY(Category = "Graphics", EditAnywhere, meta = (DisplayName = "translucencyByDistance"))
	FNearFarScalar TranslucencyByDistance;

	// How the billboard's pixel offset should change based on the billboard's distance from the camera. This scalar value will be multiplied by `pixelOffset`.
	UPROPERTY(Category = "Graphics", EditAnywhere, meta = (DisplayName = "pixelOffsetScaleByDistance"))
	FNearFarScalar PixelOffsetScaleByDistance;

	// A sub-region of the image which will be used for the billboard, rather than the entire image, measured in pixels from the bottom-left.
	UPROPERTY(Category = "Graphics", EditAnywhere, meta = (DisplayName = "imageSubRegion"))
	FBox2D ImageSubRegion;

	// The display condition specifying the distance from the camera at which this billboard will be displayed.
	UPROPERTY(Category = "Graphics", EditAnywhere, meta = (DisplayName = "distanceDisplayCondition"))
	FFloatRange DistanceDisplayCondition;

	// The distance from the camera at which to disable the depth test. This can be used to prevent clipping against terrain, for example.
	// When set to zero, the depth test is always applied. When set to Infinity, the depth test is never applied.
	UPROPERTY(Category = "Graphics", EditAnywhere, meta = (DisplayName = "distanceDisplayCondition"))
	float DisableDepthTestDistance = 0.0;

};
