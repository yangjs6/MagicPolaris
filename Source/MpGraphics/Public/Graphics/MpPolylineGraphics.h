// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MpShapeGraphics.h"
#include "MpPolylineGraphics.generated.h"


UCLASS()
class MPGRAPHICS_API UMpPolylineGraphics : public UMpShapeGraphicsPoints
{
	GENERATED_BODY()
public:
	UMpPolylineGraphics();

protected:
	virtual TSharedPtr<FMeshShapeGenerator> GenarateFillShape();

	virtual bool GenarateOutlineShape();
};
