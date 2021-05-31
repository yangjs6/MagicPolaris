#include "MpEarthActor.h"

AMpEarthActor::AMpEarthActor()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
}

bool AMpEarthActor::IsGameView()
{
	return GetWorld()->IsGameWorld();
}

UWorld* AMpEarthActor::GetWorld() const
{
	return Super::GetWorld();
}

USceneComponent* AMpEarthActor::GetRootComponent() const
{
	return Super::GetRootComponent();
}

IMpViewCamera* AMpEarthActor::GetCamera() const
{
//#if WITH_EDITOR
//	if (GWorld && GWorld->WorldType == EWorldType::Editor)
//	{
//		return WorldViewState_Editor.Get();
//	}
//	else if (GIsPlayInEditorWorld)
//	{
//		if (WorldViewState_Sequence)
//		{
//			return WorldViewState_Sequence.Get();
//		}
//	}
//#endif
//	if (OrbitCameraComponent)
//	{
//		return OrbitCameraComponent;
//	}

	return nullptr;
}
