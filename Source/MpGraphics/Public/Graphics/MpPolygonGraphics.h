// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MpShapeGraphics.h"
#include "MpPolygonGraphics.generated.h"


UCLASS()
class MPGRAPHICS_API UMpPolygonGraphics : public UMpShapeGraphicsPoints
{
	GENERATED_BODY()
public:
	UMpPolygonGraphics();

	// @param{ Property }[options.height = 0] A numeric Property specifying the altitude of the polygon relative to the ellipsoid surface.
	UPROPERTY(Category = "Shape", EditAnywhere, meta = (DisplayName = "height"))
	float Height;

	//  @param{ Property }[options.extrudedHeight] A numeric Property specifying the altitude of the polygon's extruded face relative to the ellipsoid surface.
	UPROPERTY(Category = "Shape", EditAnywhere, meta = (DisplayName = "extrudedHeight"))
	float ExtrudedHeight;

	//  @param{ Boolean }[options.closeTop = true] When false, leaves off the top of an extruded polygon open.
	UPROPERTY(Category = "Shape", EditAnywhere, meta = (DisplayName = "closeTop"))
	bool bCloseTop = false;

	//  @param{ Boolean }[options.closeBottom = true] When false, leaves off the bottom of an extruded polygon open.
	UPROPERTY(Category = "Shape", EditAnywhere, meta = (DisplayName = "closeBottom"))
	bool bCloseBottom = false;

protected:
	virtual TSharedPtr<FMeshShapeGenerator> GenarateFillShape();

	virtual bool GenarateOutlineShape();
};
