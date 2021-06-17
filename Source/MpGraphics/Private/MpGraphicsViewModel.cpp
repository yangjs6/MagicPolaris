#include "MpGraphicsViewModel.h"
#include "MpTypes.h"
#include "MpGraphics.h"
#include "Async/ParallelFor.h"


UMpGraphicsViewModel::UMpGraphicsViewModel()
{
}

UMpGraphicsViewModel::~UMpGraphicsViewModel()
{

}

void UMpGraphicsViewModel::ClearGraphics()
{
	AllGraphics.Empty();
	GraphicsToUpdate.Empty();
	GraphicsToRetrieve.Empty();
}

void UMpGraphicsViewModel::AddGraphics(UMpGraphics* Graphics)
{
	if (Graphics)
	{
		AllGraphics.Add(Graphics);
	}
}

void UMpGraphicsViewModel::OnUpdateSyn(IMpSceneView* SceneView, float DeltaTime)
{
	GraphicsToUpdate.Empty();
	GraphicsToRetrieve.Empty();

	TScriptInterface<IMpViewCamera> pViewCamera = SceneView->GetCamera();

	int NumGraphics = AllGraphics.Num();
	for (int i = 0; i < NumGraphics; i++)
	{
		UMpGraphics* pGraphics = AllGraphics[i];
		if (!pGraphics)
		{
			continue;
		}

		bool bGraphicsVisiable = pGraphics->Show;

		if (pViewCamera && pViewCamera->Intersets(pGraphics->GetBoundingBox()) < 0)
		{
			bGraphicsVisiable = false;
		}

		if (bGraphicsVisiable)
		{
			pGraphics->RegisterToDrawContext(SceneView, DrawContext);
			GraphicsToUpdate.Add(pGraphics);
		}
		else
		{
			pGraphics->UnRegisterFromDrawContext(SceneView, DrawContext);
			GraphicsToRetrieve.Add(pGraphics);
		}
	}
}

void UMpGraphicsViewModel::OnUpdateAsyn(IMpSceneView* SceneView, float DeltaTime)
{
	ParallelFor(GraphicsToUpdate.Num(), [&](int i)
	{
		UMpGraphics* pGraphics = GraphicsToUpdate[i];
		pGraphics->UpdateGraphics(SceneView, DrawContext, DeltaTime);
	}, !bUseUpdateAsyn);
}

void UMpGraphicsViewModel::OnRetrieveSyn(IMpSceneView* SceneView, float DeltaTime)
{
}

void UMpGraphicsViewModel::OnRetrieveAsyn(IMpSceneView* SceneView, float DeltaTime)
{

}

bool AMpGraphicsViewModelActor::RegisterDrawComponent(USceneComponent* Component)
{
	if (!Component->IsRegistered())
	{
		Component->SetupAttachment(GetRootComponent());
		Component->RegisterComponentWithWorld(GetWorld());
		return true;
	}
	return false;
}

bool AMpGraphicsViewModelActor::UnRegisterDrawComponent(USceneComponent* Component)
{
	if (Component->IsRegistered())
	{
		return true;
		Component->UnregisterComponent();
	}
	return false;
}

AMpGraphicsViewModelActor::AMpGraphicsViewModelActor()
{
	ViewModel = CreateDefaultSubobject<UMpGraphicsViewModel>("ViewModel");
	RootComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");

	ViewModel->InitDrawContext(this);
}

