// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "IMpApp.h"
#include "IMpSceneManager.h"
#include "IMpVertexSource.h"

class MPCORE_API FMpCoreAPI
{
public:
	static IMpApp* GetMainApp();

	static TSharedPtr<IMpSceneManager> CreateSceneManager();

	static TSharedPtr<IMpVertexSource> CreateVertexSource();
};
