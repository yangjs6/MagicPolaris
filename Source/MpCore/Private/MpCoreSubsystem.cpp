// Copyright Epic Games, Inc. All Rights Reserved.

#include "MpCoreSubsystem.h"
#include "IMpViewModel.h"
#include "MpLog.h"

UMpCoreSubsystem::UMpCoreSubsystem()
{

}

void UMpCoreSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	UE_LOG(LogMp, Log, TEXT("CoreSubsystem::Initialize"));

	SceneViews.Empty();
	ViewModels.Empty();
}

void UMpCoreSubsystem::Deinitialize()
{
	ViewModels.Empty();
	SceneViews.Empty();

	UE_LOG(LogMp, Log, TEXT("CoreSubsystem::Deinitialize"));
}

void UMpCoreSubsystem::AttachSceneView(IMpSceneView* View)
{
	if (View)
	{
		SceneViews.AddUnique(View);
	}
}

void UMpCoreSubsystem::DetachSceneView(IMpSceneView* View)
{
	if (View)
	{
		SceneViews.Remove(View);
	}
}

void UMpCoreSubsystem::AttachViewModel(IMpViewModel* Model)
{
	if (Model)
	{
		ViewModels.AddUnique(Model);
	}
}

void UMpCoreSubsystem::DetachViewModel(IMpViewModel* Model)
{
	if (Model)
	{
		ViewModels.Remove(Model);
	}
}

void UMpCoreSubsystem::Tick(float DeltaTime)
{
	int NumView = GetSceneViewNum();
	for (int i = 0; i < NumView; i++)
	{
		IMpSceneView* SceneView = GetSceneView(i);
		UpdateViewModels(SceneView, DeltaTime);
	}
}

TStatId UMpCoreSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UMpEarthSubsystem, STATGROUP_Tickables);
}

void UMpCoreSubsystem::UpdateViewModels(IMpSceneView* SceneView, float DeltaTime)
{
	int NumModel = GetViewModelNum();

	for (int i = 0; i < NumModel; i++)
	{
		IMpViewModel* ViewModel = GetViewModel(i);
		ViewModel->OnUpdateSyn(SceneView, DeltaTime);
	}

	for (int i = 0; i < NumModel; i++)
	{
		IMpViewModel* ViewModel = GetViewModel(i);
		ViewModel->OnUpdateAsyn(SceneView, DeltaTime);
	}

	for (int i = 0; i < NumModel; i++)
	{
		IMpViewModel* ViewModel = GetViewModel(i);
		if (ViewModel->OnPreDraw(SceneView))
		{
			ViewModel->OnPostDraw(SceneView);
		}
	}

	for (int i = 0; i < NumModel; i++)
	{
		IMpViewModel* ViewModel = GetViewModel(i);
		ViewModel->OnRetrieveSyn(SceneView, DeltaTime);
	}

	for (int i = 0; i < NumModel; i++)
	{
		IMpViewModel* ViewModel = GetViewModel(i);
		ViewModel->OnRetrieveAsyn(SceneView, DeltaTime);
	}
}
