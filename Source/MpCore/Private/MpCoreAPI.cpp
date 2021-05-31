// Copyright Epic Games, Inc. All Rights Reserved.

#include "MpCoreAPI.h"
#include "MpCoreSubsystem.h"
#include "MpSceneManager.h"
#include "MpVertexSource.h"

IMpApp* FMpCoreAPI::GetMainApp()
{
	return GEngine->GetEngineSubsystem<UMpCoreSubsystem>();
}

TSharedPtr<IMpSceneManager> FMpCoreAPI::CreateSceneManager()
{
	return MakeShareable<IMpSceneManager>(new CMpSceneManager());
}

TSharedPtr<IMpVertexSource> FMpCoreAPI::CreateVertexSource()
{
	return MakeShareable<IMpVertexSource>(new CMpVertexSource());
}
