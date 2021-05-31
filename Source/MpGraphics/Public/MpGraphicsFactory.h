// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

class UMpShapeGraphics;
class UMpEntity;

class MPGRAPHICS_API FMpGraphicsFactory
{
public:
	template<class TMpGraphics>
	static TMpGraphics* CreateGraphics();
};


template<class TMpGraphics>
TMpGraphics* FMpGraphicsFactory::CreateGraphics()
{
	TMpGraphics* NewGraphics = NewObject<TMpGraphics>();

	return NewGraphics;
}
