// Copyright Epic Games, Inc. All Rights Reserved.

#include "MpViewSubsystem.h"
#include "MpCoreAPI.h"

UMpViewSubsystem::UMpViewSubsystem()
{
	//SceneView = CreateDefaultSubobject<UMpLocalPlayerSceneView>("SceneView");
}

void UMpViewSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	UE_LOG(LogMp, Log, TEXT("ViewSubsystem::Initialize"));

//	IMpApp* MainApp = FMpCoreAPI::GetMainApp();
//
//	MainApp->AttachSceneView(SceneView);
//
//	ULocalPlayer* LocalPlayer = GetLocalPlayer();
//	SceneView->AttachLocalPlayer(LocalPlayer);
}

void UMpViewSubsystem::Deinitialize()
{
	//SceneView->DetachLocalPlayer();

	//IMpApp* MainApp = FMpCoreAPI::GetMainApp();

	//MainApp->DetachSceneView(SceneView);

	UE_LOG(LogMp, Log, TEXT("ViewSubsystem::Deinitialize"));
}

//bool UMpLocalPlayerSceneView::IsPointVisible(const FVector& Position)
//{
//	return false;
//}
//
//bool UMpLocalPlayerSceneView::ComputeScreenSpacePosition(const FVector& InPosition, FVector2D& OutPosition)
//{
//	if (LocalPlayer.IsValid())
//	{
//		return LocalPlayer->GetPixelPoint(InPosition, OutPosition);
//	}
//	return false;
//}

//void UMpLocalPlayerSceneView::AttachLocalPlayer(ULocalPlayer* InLocalPlayer)
//{
//	LocalPlayer = InLocalPlayer;
//}
//
//void UMpLocalPlayerSceneView::DetachLocalPlayer()
//{
//	LocalPlayer = nullptr;
//}
