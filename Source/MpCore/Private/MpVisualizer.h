// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MpVisualizer.generated.h"

class IMpSceneView;
/*
*/
UCLASS(Abstract)
class UMpVisualizer : public UObject
{
	GENERATED_BODY()
public:
	virtual void UpdateEntities( float Time) PURE_VIRTUAL(,);
	virtual void UpdateView(IMpSceneView* SceneView) PURE_VIRTUAL(, );
};
