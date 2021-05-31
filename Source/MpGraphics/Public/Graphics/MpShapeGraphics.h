// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MpGraphics.h"
#include "MpShapeGraphics.generated.h"

class FMeshShapeGenerator;

UCLASS(Abstract)
class UMpShapeGraphics : public UMpGraphicsBase
{
	GENERATED_BODY()
public:
	// @property {Property | HeightReference}[heightReference = HeightReference.NONE] A Property specifying what the height from the entity position is relative to.
	
	// @property {Property | boolean}[fill = true] A boolean Property specifying whether the shape is filled with the provided material.
	UPROPERTY(Category = "Shape", EditAnywhere, meta = (DisplayName = "fill"))
	bool bFill = true;

	// @property {MaterialProperty | Color}[material = Color.WHITE] A Property specifying the material used to fill the shape.
	
	// @property {Property | boolean}[outline = false] A boolean Property specifying whether the shape is outlined.
	UPROPERTY(Category = "Shape", EditAnywhere, meta = (DisplayName = "outline"))
	bool bOutline = true;

	// @property {Property | Color}[outlineColor = Color.BLACK] A Property specifying the{ @link Color } of the outline.
	UPROPERTY(Category = "Shape", EditAnywhere, meta = (DisplayName = "OutlineColor"))
	FColor OutlineColor = FColor(0, 0, 0, 0);

	// @property {Property | number}[outlineWidth = 1.0] A numeric Property specifying the width of the outline.
	UPROPERTY(Category = "Shape", EditAnywhere, meta = (DisplayName = "outlineWidth"))
	float OutlineWidth = 1.0;

	// @property {Property | ShadowMode}[shadows = ShadowMode.DISABLED] An enum Property specifying whether the shape casts or receives shadows from light sources.
	
	// @property {Property | DistanceDisplayCondition}[distanceDisplayCondition] A Property specifying at what distance from the camera that this shape will be displayed.


protected:

	virtual void UpdateGeometry(IMpSceneView* pView, float DeltaTime);

	virtual TSharedPtr<FMeshShapeGenerator> GenarateFillShape() { return TSharedPtr<FMeshShapeGenerator>(); }

	virtual bool GenarateOutlineShape() { return false; }
};

UCLASS(Abstract)
class UMpShapeGraphicsPoint : public UMpShapeGraphics
{
	GENERATED_BODY()
public:
	// @property {PositionProperty | Cartesian3}[position] A Property specifying the graphics position.
	UPROPERTY(Category = "Graphics", EditAnywhere, meta = (DisplayName = "position"))
	FGeoPosition Position;


protected:
	virtual void UpdatePosition(IMpSceneView* pView, float DeltaTime);

	FGeoFrame LocalFrame;
};

UCLASS(Abstract)
class UMpShapeGraphicsPoints : public UMpShapeGraphics
{
	GENERATED_BODY()
public:
	// @property {PositionsProperty | Cartesian3}[positions] A Property specifying the graphics positions.
	UPROPERTY(Category = "Graphics", EditAnywhere, meta = (DisplayName = "positions"))
	TArray<FGeoPosition> Positions;

protected:
	virtual void UpdatePosition(IMpSceneView* pView, float DeltaTime);

	TArray<FGeoCartesian> Cartesians;

};
