#include "MpEarthEditorSubsystem.h"
#include "MpCoreAPI.h"
#include "MpLog.h"

UMpEarthEditorSubsystem::UMpEarthEditorSubsystem()
{
	//SceneView = CreateDefaultSubobject<UMpEditorSceneView>("SceneView");
}

void UMpEarthEditorSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	UE_LOG(LogMp, Log, TEXT("EarthEditorSubsystem::Initialize"));

	//IMpApp* MainApp = FMpCoreAPI::GetMainApp();

	//MainApp->AttachSceneView(SceneView);

}

void UMpEarthEditorSubsystem::Deinitialize()
{
	IMpApp* MainApp = FMpCoreAPI::GetMainApp();

	//MainApp->DetachSceneView(SceneView);

	//UE_LOG(LogMp, Log, TEXT("EarthEditorSubsystem::Deinitialize"));
}

//bool UMpEditorSceneView::IsPointVisible(const FVector& Position)
//{
//	return false;
//}
//
//bool UMpEditorSceneView::ComputeScreenSpacePosition(const FVector& InPosition, FVector2D& OutPosition)
//{
//	return false;
//}
