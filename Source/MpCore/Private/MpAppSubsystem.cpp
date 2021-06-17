// Copyright Epic Games, Inc. All Rights Reserved.

#include "MpAppSubsystem.h"
#include "MpEarthActor.h"
#include "Interfaces/IMpViewModel.h"
#include "MpLog.h"
#include "EngineUtils.h"

UMpAppSubsystem::UMpAppSubsystem()
{
	bInitialized = false;
}

void UMpAppSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	UE_LOG(LogMp, Log, TEXT("CoreSubsystem::Initialize"));

	bInitialized = true;
}

void UMpAppSubsystem::Deinitialize()
{
	UE_LOG(LogMp, Log, TEXT("CoreSubsystem::Deinitialize"));

	bInitialized = false;
}

UMpAppSubsystem* UMpAppSubsystem::GetCoreSubsystem(const UWorld* InWorld)
{
	if (InWorld)
	{
		return InWorld->GetSubsystem<UMpAppSubsystem>();
	}

	return nullptr;
}

FMpViewModelManager* UMpAppSubsystem::GetViewModelManager(UWorld* InWorld)
{
	if (UMpAppSubsystem* Subsystem = GetCoreSubsystem(InWorld))
	{
		return Subsystem->GetViewModelManager();
	}

	return nullptr;
}

FMpViewModelManager* UMpAppSubsystem::GetViewModelManager()
{
	return &ViewModelManager;
}

AMpEarthActor* UMpAppSubsystem::GetEarthActor()
{
	if (UWorld* World = GetWorld())
	{
		// @todo: this assumes only one water mesh actor right now. 
		TActorIterator<AMpEarthActor> It(World);
		EarthActor = It ? *It : nullptr;

		return EarthActor;
	}

	return nullptr;
}

void UMpAppSubsystem::AttachSceneView(IMpSceneView* View)
{
}

void UMpAppSubsystem::DetachSceneView(IMpSceneView* View)
{
}

int UMpAppSubsystem::GetSceneViewNum()
{
	return 1;
}

IMpSceneView* UMpAppSubsystem::GetSceneView(int i)
{
	return GetEarthActor();
}

void UMpAppSubsystem::AttachViewModel(IMpViewModel* Model)
{
	ViewModelManager.AttachViewModel(Model);
}

void UMpAppSubsystem::DetachViewModel(IMpViewModel* Model)
{
	ViewModelManager.DetachViewModel(Model);
}

int UMpAppSubsystem::GetViewModelNum()
{
	return ViewModelManager.GetViewModelNum();
}

IMpViewModel* UMpAppSubsystem::GetViewModel(int i)
{
	return ViewModelManager.GetViewModel(i);
}

void UMpAppSubsystem::Tick(float DeltaTime)
{
	if (!GetEarthActor())
	{
		return;
	}

	ViewModelManager.UpdateViewModels(EarthActor, DeltaTime);
}

TStatId UMpAppSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UMpEarthSubsystem, STATGROUP_Tickables);
}

bool UMpAppSubsystem::DoesSupportWorldType(EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::Game || WorldType == EWorldType::Editor || WorldType == EWorldType::PIE;
}

