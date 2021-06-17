// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MpGraphics.h"
#include "MpShapeGraphics.generated.h"

class FMeshShapeGenerator;

class UMpTriangleSet;
class UMpTriangleSetComponent;
class UMpLineSetComponent;

UCLASS(Abstract)
class UMpShapeGraphics : public UMpGraphics
{
	GENERATED_BODY()
public:
	
	UMpShapeGraphics();

	// @property {Property | HeightReference}[heightReference = HeightReference.NONE] A Property specifying what the height from the entity position is relative to.
	
	// @property {Property | boolean}[fill = true] A boolean Property specifying whether the shape is filled with the provided material.
	UPROPERTY(Category = "Shape", EditAnywhere, meta = (DisplayName = "fill"))
	bool bFill = true;

	// @property {Property | Color}[fillColor = Color.WHITE] A Property specifying the color used to fill the shape.
	UPROPERTY(Category = "Shape", EditAnywhere, meta = (DisplayName = "fillColor"))
	FColor FillColor = FColor(0, 0, 0, 0);

	// @property {StyleProperty}[style = grid] A Property specifying the style used to fill the shape.
	UPROPERTY(Category = "Shape", EditAnywhere, meta = (DisplayName = "fillMaterial"))
	FName FillStyle;

	// @property {Property | boolean}[outline = false] A boolean Property specifying whether the shape is outlined.
	UPROPERTY(Category = "Shape", EditAnywhere, meta = (DisplayName = "outline"))
	bool bOutline = true;

	// @property {Property | Color}[outlineColor = Color.BLACK] A Property specifying the{ @link Color } of the outline.
	UPROPERTY(Category = "Shape", EditAnywhere, meta = (DisplayName = "outlineColor"))
	FColor OutlineColor = FColor(1, 1, 1, 1);

	// @property {Property | number}[outlineWidth = 1.0] A numeric Property specifying the width of the outline.
	UPROPERTY(Category = "Shape", EditAnywhere, meta = (DisplayName = "outlineWidth"))
	float OutlineWidth = 1.0;

	// @property {Property | ShadowMode}[shadows = ShadowMode.DISABLED] An enum Property specifying whether the shape casts or receives shadows from light sources.
	
	// @property {Property | DistanceDisplayCondition}[distanceDisplayCondition] A Property specifying at what distance from the camera that this shape will be displayed.


protected:

	virtual void UpdateGeometry(IMpSceneView* pView, IMpGraphicsDrawContext* DrawContext, float DeltaTime);
	virtual void UpdateMaterial(IMpSceneView* pView, IMpGraphicsDrawContext* DrawContext, float DeltaTime);

	virtual TSharedPtr<FMeshShapeGenerator> GenarateFillShape() { return TSharedPtr<FMeshShapeGenerator>(); }

	virtual bool GenarateOutlineShape() { return false; }

	//! 准备绘制图形
	virtual void RegisterToDrawContext(IMpSceneView* pView, IMpGraphicsDrawContext* DrawContext);

	//! 结束绘制图形
	virtual void UnRegisterFromDrawContext(IMpSceneView* pView, IMpGraphicsDrawContext* DrawContext);

	UPROPERTY(Transient)
		UMpTriangleSet* TriangleSet;

	UPROPERTY(Transient)
		UMpTriangleSetComponent* TriangleSetComponent;

	UPROPERTY(Transient)
		UMpLineSetComponent* LineSetComponent;

	FGeoFrame LocalFrame;
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

	virtual void UpdateBoundingBox();
	virtual void UpdatePosition(IMpSceneView* pView, IMpGraphicsDrawContext* DrawContext, float DeltaTime);

	FVector UECoordinate;
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
	virtual void UpdateBoundingBox();
	virtual void UpdatePosition(IMpSceneView* pView, IMpGraphicsDrawContext* DrawContext, float DeltaTime);

	TArray<FVector> UECoordinates;
};
