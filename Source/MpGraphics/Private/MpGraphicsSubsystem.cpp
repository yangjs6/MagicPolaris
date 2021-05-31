// Copyright Epic Games, Inc. All Rights Reserved.

#include "MpGraphicsSubsystem.h"
#include "MpGraphicsViewModel.h"
#include "MpCoreAPI.h"
#include "MpLog.h"

UMpGraphicsSubsystem::UMpGraphicsSubsystem()
{

}

void UMpGraphicsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	UE_LOG(LogMp, Log, TEXT("GraphicsSubsystem::Initialize"));
	FMpCoreAPI::GetMainApp()->AttachViewModel(FMpGraphicsViewModel::Get());
}

void UMpGraphicsSubsystem::Deinitialize()
{
	FMpCoreAPI::GetMainApp()->DetachViewModel(FMpGraphicsViewModel::Get());
	UE_LOG(LogMp, Log, TEXT("GraphicsSubsystem::Deinitialize"));
}
