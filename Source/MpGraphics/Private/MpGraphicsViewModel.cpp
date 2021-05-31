#include "MpGraphicsViewModel.h"
#include "MpTypes.h"
#include "MpCoreAPI.h"
#include "MpGraphics.h"
#include "IMpVertexSource.h"
#include "IMpSceneView.h"
#include "Async/ParallelFor.h"

FMpGraphicsViewModel GGraphicsViewModel;

FString FMpGraphicsViewModel::DefaultSceneName = "DefaultScene";
FString FMpGraphicsViewModel::DefaultLayerName = "DefaultLayer";
FString FMpGraphicsViewModel::DefaultNodeName = "DefaultNode";

FMpGraphicsViewModel* FMpGraphicsViewModel::Get()
{
	return &GGraphicsViewModel;
}

FMpGraphicsViewModel::FMpGraphicsViewModel()
{
	SceneManager = FMpCoreAPI::CreateSceneManager();
}

FMpGraphicsViewModel::~FMpGraphicsViewModel()
{

}

void FMpGraphicsViewModel::AddReferencedObjects(FReferenceCollector& Collector)
{
	Collector.AddReferencedObjects(AddedGraphics);
}

void FMpGraphicsViewModel::AddGraphics(UMpGraphicsBase* Graphics, const FString& NodeName, const FString& LayerName, const FString& SceneName)
{
	if (!Graphics)
	{
		return;
	}

	IMpScene* pScene = FMpSceneUtils::GetOrCreateScene(GetSceneManager(), SceneName);
	IMpLayer* pLayer = FMpSceneUtils::GetOrCreateLayer(GetSceneManager(), pScene, LayerName);
	IMpNode* pNode = FMpSceneUtils::CreateNode(Graphics, GetSceneManager(), pLayer, NodeName);

	if (pNode)
	{
		AddedGraphics.Add(Graphics);
	}
}

void FMpGraphicsViewModel::OnUpdateSyn(IMpSceneView* SceneView, float DeltaTime)
{
	IMpViewCamera* pViewCamera = SceneView->GetCamera();

	int nScene = SceneManager->GetObjectCount();
	for (int s = 0; s < nScene; s++)
	{
		IMpScene* pScene = SceneManager->GetObjectByIdx(s);
		if (!pScene->GetVisiable())
		{
			continue;
		}
		int nLayer = pScene->GetObjectCount();
		//for (int i=0;i<nLayer;i++)
		for (int i = nLayer - 1; i >= 0; i--)
		{
			IMpLayer* pLayer = pScene->GetObjectByIdx(i);
			if (!pLayer->GetVisiable())
			{
				continue;
			}
			int nNode = pLayer->GetObjectCount();
			//for (int j=0;j<nNode;j++)
			for (int j = nNode - 1; j >= 0; j--)
			{
				IMpNode* pNode = pLayer->GetObjectByIdx(j);

				if (!pNode->GetVisiable())
				{
					continue;
				}

				UMpGraphicsBase* pGraphics = (UMpGraphicsBase*)pNode->GetGraphics();
				if (!pGraphics)
				{
					continue;
				}
				IMpVertexSource* pVertexSource = pGraphics->GetVertexSource();
				if (pVertexSource && pViewCamera &&
					pViewCamera->Intersets(pVertexSource->GetBoundingBox()) < 0)
				{
					continue;
				}


				GraphicsToUpdate.Add(pGraphics);

			}
		}
	}
}

void FMpGraphicsViewModel::OnUpdateAsyn(IMpSceneView* SceneView, float DeltaTime)
{
	ParallelFor(GraphicsToUpdate.Num(), [&](int i)
	{
		UMpGraphicsBase* pGraphics = GraphicsToUpdate[i];
		pGraphics->UpdateGraphics(SceneView, DeltaTime);
	});
}

bool FMpGraphicsViewModel::OnPreDraw(IMpSceneView* SceneView)
{
	for (int i = 0; i < GraphicsToUpdate.Num(); i++)
	{
		UMpGraphicsBase* pGraphics = GraphicsToUpdate[i];
		pGraphics->PreDrawGraphics(SceneView, DrawContext);
	}
	return true;
}

void FMpGraphicsViewModel::OnPostDraw(IMpSceneView* SceneView)
{
	for (int i = 0; i < GraphicsToUpdate.Num(); i++)
	{
		UMpGraphicsBase* pGraphics = GraphicsToUpdate[i];
		pGraphics->PostDrawGraphics(SceneView, DrawContext);
	}
}

void FMpGraphicsViewModel::OnRetrieveSyn(IMpSceneView* SceneView, float DeltaTime)
{
	GraphicsToUpdate.Empty();
}

void FMpGraphicsViewModel::OnRetrieveAsyn(IMpSceneView* SceneView, float DeltaTime)
{

}
