#include "MpViewModelManager.h"
#include "Interfaces/IMpViewModel.h"

void FMpViewModelManager::UpdateViewModels(IMpSceneView* SceneView, float DeltaTime)
{
	TArray<IMpViewModel*> EnalbedViewModels;

	for (int i = 0; i < ViewModels.Num(); i++)
	{
		if (ViewModels[i]->IsEnable(SceneView))
		{
			EnalbedViewModels.Add(ViewModels[i]);
		}
	}

	int NumModel = EnalbedViewModels.Num();

	for (int i = 0; i < NumModel; i++)
	{
		EnalbedViewModels[i]->OnUpdateSyn(SceneView, DeltaTime);
	}

	for (int i = 0; i < NumModel; i++)
	{
		EnalbedViewModels[i]->OnUpdateAsyn(SceneView, DeltaTime);
	}

	for (int i = 0; i < NumModel; i++)
	{
		if(EnalbedViewModels[i]->OnPreDraw(SceneView))
		{
			EnalbedViewModels[i]->OnPostDraw(SceneView);
		}
	}

	for (int i = 0; i < NumModel; i++)
	{
		EnalbedViewModels[i]->OnRetrieveSyn(SceneView, DeltaTime);
	}

	for (int i = 0; i < NumModel; i++)
	{
		EnalbedViewModels[i]->OnRetrieveAsyn(SceneView, DeltaTime);
	}
}

void FMpViewModelManager::AttachViewModel(IMpViewModel* ViewModel)
{
	ViewModels.AddUnique(ViewModel);
}

void FMpViewModelManager::DetachViewModel(IMpViewModel* ViewModel)
{
	ViewModels.Remove(ViewModel);
}
