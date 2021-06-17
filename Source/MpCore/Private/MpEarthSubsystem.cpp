// Copyright Epic Games, Inc. All Rights Reserved.

#include "MpEarthSubsystem.h"
#include "MpLog.h"

UMpEarthSubsystem::UMpEarthSubsystem()
{

}

void UMpEarthSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	UE_LOG(LogMp, Log, TEXT("EarthSubsystem::Initialize"));

}

void UMpEarthSubsystem::Deinitialize()
{

	UE_LOG(LogMp, Log, TEXT("EarthSubsystem::Deinitialize"));
}

AMpEarthActor* UMpEarthSubsystem::GetEarthActor()
{
	return EarthActor;
}
