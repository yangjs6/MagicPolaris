// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MpShapeGraphics.h"
#include "MpBoxGraphics.generated.h"


UCLASS()
class MPGRAPHICS_API UMpBoxGraphics : public UMpShapeGraphicsPoint
{
	GENERATED_BODY()
public:
	UMpBoxGraphics();

	// @property {Property | Cartesian3}[dimensions] A{ @link Cartesian3 } Property specifying the length, width, and height of the shape.
	UPROPERTY(Category = "Shape", EditAnywhere, meta = (DisplayName = "dimensions"))
	FVector Dimensions;

	/** If true (default), UVs are scaled so that there is no stretching. If false, UVs are scaled to fill unit square */
	UPROPERTY(Category = "Shape", EditAnywhere, meta = (DisplayName = "scaleUV"))
	bool bScaleUVByAspectRatio = true;

protected:
	virtual TSharedPtr<FMeshShapeGenerator> GenarateFillShape();

	virtual bool GenarateOutlineShape();
};
