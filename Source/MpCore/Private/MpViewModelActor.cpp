#include "MpViewModelActor.h"
#include "MpAppSubsystem.h"

void AMpViewModelActor::PostRegisterAllComponents()
{
	Super::PostRegisterAllComponents();

	IMpViewModel* ViewModel = GetViewModel();
	UMpAppSubsystem* Manager = UMpAppSubsystem::GetCoreSubsystem(GetWorld());
	if (ViewModel && Manager && !IsTemplate())
	{
		Manager->AttachViewModel(ViewModel);
	}
}


void AMpViewModelActor::PostUnregisterAllComponents()
{
	Super::PostUnregisterAllComponents();

	IMpViewModel* ViewModel = GetViewModel();
	UMpAppSubsystem* Manager = UMpAppSubsystem::GetCoreSubsystem(GetWorld());
	if (ViewModel && Manager && !IsTemplate())
	{
		Manager->DetachViewModel(ViewModel);
	}
}
