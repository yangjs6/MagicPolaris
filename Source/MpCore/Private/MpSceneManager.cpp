#include "MpSceneManager.h"

const FString EmptyString;

//////////////////////////////////////////////////////////////////////////

void CMpSceneManager::doAddScene(IMpScene* pScene)
{
	AddObject(pScene);
	OnCreateScene(pScene->GetID());
	SelectScene(pScene);
}

//! ��������
IMpScene* CMpSceneManager::CreateScene(int sceneId, const FString& name)
{
	IMpScene* pScene = this->GetObjectById(sceneId);
	if (pScene) return pScene;

	pScene = mSceneFactory->CreateScene(sceneId, name);

	doAddScene(pScene);

	pScene->Release();
	return pScene;
}

//! ɾ������
void CMpSceneManager::doDeleteScene(IMpScene* pScene)
{
	if (pScene == nullptr)
		return;

	OnDeleteScene(pScene->GetID());

	ClearSelectedNodes();
	if (mActiveSceneID == pScene)
		SelectNothing();

	RemoveObject(pScene);
}

//! ɾ������
void CMpSceneManager::DeleteScene(IMpScene* pScene)
{
	doDeleteScene(pScene);
}

//! ѡ�г���
void CMpSceneManager::SelectScene(IMpScene* pScene)
{
	if (pScene == nullptr)
		return SelectNothing();

	mActiveSceneID = pScene;
	mActiveLayerID = nullptr;
	if (mActiveNodeID)
		mActiveNodeID->SetEditable(false);
	mActiveNodeID = nullptr;

	ClearSelectedNodes();

	OnSelectScene(pScene->GetID());
}

//! ��ճ���
void CMpSceneManager::ClearScene(IMpScene* pScene)
{
	if (pScene == nullptr)
		return;

	// ����ɾ��ͼ�������еĽڵ�
	while (pScene->GetObjectCount() > 0)
	{
		DeleteLayer(pScene->GetObjectByIdx(0));
	}
	//pScene->ClearObjects();
	OnClearScene(pScene->GetID());

}

//! ���ó�������
void CMpSceneManager::SetSceneName(IMpScene* pScene, const FString& name)
{
	if (pScene)
	{
		pScene->SetName(name);
		OnSceneUpdated(pScene->GetID(), 1);
	}
}

//! ����ID��ó���
IMpScene* CMpSceneManager::GetObjectById(int id)
{
	int nObj = this->GetObjectCount();
	for (int i = 0; i < nObj; i++)
	{
		IMpScene* pObject = GetObjectByIdx(i);
		if (pObject->GetID() == id)
		{
			return pObject;
		}
	}
	return nullptr;
}

//! ����ID��ó���
IMpScene* CMpSceneManager::GetSceneByID(int sceneId)
{
	return GetObjectById(sceneId);
}

//! �������ƻ�ó���
IMpScene* CMpSceneManager::GetSceneByName(const FString& sceneName)
{
	int nObj = this->GetObjectCount();
	for (int i = 0; i < nObj; i++)
	{
		IMpScene* pObject = GetObjectByIdx(i);
		if (pObject->GetName() == sceneName)
		{
			return pObject;
		}
	}
	return nullptr;
}

//! ���ݳ������ֵõ�ID
int CMpSceneManager::GetSceneIDByName(const FString& mame)
{
	IMpScene* pScene = GetSceneByName(mame);
	if (pScene)
		return pScene->GetID();
	else return 0;
}

//! ���ݳ���ID�õ�����
const FString& CMpSceneManager::GetSceneNameByID(int id)
{
	IMpScene* pScene = GetSceneByID(id);
	if (pScene)
		return pScene->GetName();
	else return EmptyString;
}

//////////////////////////////////////////////////////////////////////////

//! ����ͼ��
void CMpSceneManager::doAddLayer(IMpScene* pScene, IMpLayer* pLayer)
{
	pLayer->AttachScene(pScene);
	pScene->AddObject(pLayer);
	OnAddLayer(pScene->GetID(), pLayer->GetID());
	//SelectLayer(pLayer);
}

//! ����ͼ��
IMpLayer* CMpSceneManager::CreateLayer(IMpScene* pScene, int layerId, const FString& name)
{
	IMpLayer* pLayer = pScene->GetObjectById(layerId);
	if (pLayer) return pLayer;

	pLayer = mSceneFactory->CreateLayer(layerId, name);

	doAddLayer(pScene, pLayer);

	pLayer->Release();
	return pLayer;
}

//! ɾ��ͼ��
void CMpSceneManager::doDeleteLayer(IMpLayer* pLayer)
{
	if (pLayer == nullptr)
		return;
	IMpScene* pScene = pLayer->GetScene();
	if (pScene == nullptr)
		return;
	OnDeleteLayer(pLayer->GetID());

	ClearSelectedNodes();
	if (mActiveLayerID == pLayer)
		SelectScene(pScene);

	pScene->RemoveObject(pLayer);
}

//! ɾ��ͼ��
void CMpSceneManager::DeleteLayer(IMpLayer* pLayer)
{
	doDeleteLayer(pLayer);
}

//! ����ͼ������
void CMpSceneManager::SetLayerName(IMpLayer* pLayer, const FString& name)
{
	pLayer->SetName(name);
	OnLayerUpdated(pLayer->GetID(), 0);
}

//! ����ͼ��ɼ�
void CMpSceneManager::SetLayerVisiable(IMpLayer* pLayer, bool visible)
{
	pLayer->SetVisiable(visible);
	OnSetLayerVisiable(pLayer->GetID(), visible);
}

//! ����ͼ��ɱ༭
void CMpSceneManager::SetLayerEditable(IMpLayer* pLayer, bool editable)
{
	pLayer->SetEditable(editable);
	OnSetLayerEditable(pLayer->GetID(), editable);
}

//! ѡ��ͼ��
void CMpSceneManager::SelectLayer(IMpLayer* pLayer)
{
	if (pLayer == nullptr)
		return SelectNothing();

	mActiveLayerID = pLayer;
	mActiveSceneID = mActiveLayerID->GetScene();
	if (mActiveNodeID)
		mActiveNodeID->SetEditable(false);
	mActiveNodeID = nullptr;

	ClearSelectedNodes();

	OnSelectLayer(pLayer->GetID());
}

//! ���ͼ��
void CMpSceneManager::ClearLayer(IMpLayer* pLayer)
{
	if (pLayer == nullptr) return;

	// ����ɾ��ͼ�������еĽڵ�
	while (pLayer->GetObjectCount() > 0)
	{
		DeleteNode(pLayer->GetObjectByIdx(0));
	}
	//pLayer->ClearObjects();
	OnClearLayer(pLayer->GetID());

}

//! ����ID���ͼ�㣬���sceneIdΪ0�����г����в��ң�����ֻ���ҵ�������
IMpLayer* CMpSceneManager::GetLayerByID(int layerId, int sceneId)
{
	if (layerId <= 0)
		return nullptr;

	if (sceneId > 0)
	{
		IMpScene* pScene = GetObjectById(sceneId);
		if (pScene)
			return pScene->GetObjectById(layerId);
	}
	else
	{
		int nScene = GetObjectCount();
		for (int i = 0; i < nScene; i++)
		{
			IMpScene* pScene = GetObjectByIdx(i);
			IMpLayer* pLayer = pScene->GetObjectById(layerId);
			if (pLayer) return pLayer;
		}
	}
	return nullptr;
}

//! �������ƻ��ͼ��
IMpLayer* CMpSceneManager::GetLayerByName(const FString& LayerName)
{
	int nObj = this->GetObjectCount();
	for (int i = 0; i < nObj; i++)
	{
		IMpScene* pObject = GetObjectByIdx(i);
		IMpLayer* pLayer = pObject->GetLayerByName(LayerName);
		if (pLayer) return pLayer;
	}
	return nullptr;
}

//! ����ͼ�����ֵõ�ID
int CMpSceneManager::GetLayerIDByName(const FString& mame)
{
	IMpLayer* pLayer = GetLayerByName(mame);
	if (pLayer)
		return pLayer->GetID();
	else return 0;
}

//! ����ͼ��ID�õ�����
const FString& CMpSceneManager::GetLayerNameByID(int id)
{
	IMpLayer* pLayer = GetLayerByID(id);
	if (pLayer)
		return pLayer->GetName();
	else return EmptyString;
}
//////////////////////////////////////////////////////////////////////////

//! ���ͼ��
void CMpSceneManager::doAddNode(IMpLayer* pLayer, IMpNode* pNode)
{
	IMpLayer* pLayerOld = pNode->GetLayer();
	if (pLayerOld == pLayer) return;

	// ��ɾ���Ͻڵ�
	if (pLayerOld)
	{
		OnDeleteNode(pNode->GetID());
		pLayerOld->RemoveObject(pNode);
	}

	// ��ӵ��µĲ���
	if (pLayer)
	{
		pNode->RegisterEventListener(this);
		pNode->AttachLayer(pLayer);
		pLayer->AddObject(pNode);

		OnAddNode(pLayer->GetID(), pNode->GetID());
	}
}

//! ���ͼ��
void CMpSceneManager::AddNode(IMpLayer* pLayer, IMpNode* pNode)
{
	doAddNode(pLayer, pNode);
}

//! ɾ��ͼ��
void CMpSceneManager::doDeleteNode(IMpNode* pNode)
{
	if (pNode)
	{
		IMpLayer* pLayer = pNode->GetLayer();
		if (pLayer == nullptr) return;
		if (mActiveNodeID == pNode)
		{
			SelectLayer(pLayer);
		}
		pNode->UnRegisterEventListener(this);
		OnDeleteNode(pNode->GetID());
		if (pNode->GetEditable())
			SelectNodeEx(pNode, false);

		pNode->AttachLayer(nullptr);
		pLayer->RemoveObject(pNode);
	}
}

//! ɾ��ͼ��
void CMpSceneManager::DeleteNode(IMpNode* pNode)
{
	doDeleteNode(pNode);
}
void CMpSceneManager::DeleteSelectedNodes()
{
}

//! ����ͼ�Σ��ӽڵ㵽�ı�
FString CMpSceneManager::CopyNode(IMpNode* pNode)
{
	return FString();
}

//! ճ��ͼ�Σ����ı����ڵ�
IMpNode* CMpSceneManager::ParseNode(const FString& pNodeString)
{
	return nullptr;
}

//! ����ͼ������
void CMpSceneManager::SetNodeName(IMpNode* pNode, const FString& name)
{
	pNode->SetName(name);
	OnNodeUpdated(pNode->GetID(), 0);
}

//! ����ͼ�οɼ�
void CMpSceneManager::SetNodeVisible(IMpNode* pNode, bool visible)
{
	if (pNode->GetVisiable() != visible)
	{
		pNode->SetVisiable(visible);
		OnSetNodeVisible(pNode->GetID(), visible);
	}
}

//! ����ͼ�οɱ༭
void CMpSceneManager::SetNodeEditable(IMpNode* pNode, bool editable)
{
	if (pNode->GetEditable() != editable)
	{
		pNode->SetEditable(editable);
		OnSetNodeVisible(pNode->GetID(), editable);
	}
}

//! ѡ��ͼ��
void CMpSceneManager::SelectNode(IMpNode* pNode)
{
	ClearSelectedNodes();
	SelectNodeEx(pNode);
}

//! ѡ�нڵ���չ�ӿڣ�Ϊ��ʵ�ֶ�ѡ�ڵ�
bool CMpSceneManager::SelectNodeEx(IMpNode* pNode, bool bSelect)
{
	if (pNode == nullptr)
	{
		doSelectNode(nullptr);
		return false;
	}

	if (!bSelect)
	{
		pNode->SetEditable(false);
		int NodeIndex = SelectedNodes.Find(pNode);
		if (NodeIndex >= 0)
		{
			SelectedNodes.Remove(pNode);
			NodeIndex--;
			if (SelectedNodes.Num() >0)
			{
				if (NodeIndex >=0 && NodeIndex < SelectedNodes.Num())
				{
					doSelectNode(SelectedNodes[NodeIndex]);
				}
				else
				{
					doSelectNode(SelectedNodes[0]);
				}
			}
			return true;
		}
		return false;
	}else
	{
		SelectedNodes.Add(pNode);
		doSelectNode(pNode);
		return true;
	}
}

//! ѡ��ͼ��
void CMpSceneManager::doSelectNode(IMpNode* pNode)
{
	if (mActiveNodeID == pNode && pNode != nullptr && mActiveLayerID == pNode->GetLayer())
	{
		SetNodeEditable(mActiveNodeID, true);
		return;
	}

	//if (mActiveNodeID)
	//	SetNodeEditable(mActiveNodeID,false);
	if (mActiveNodeID != nullptr && pNode == nullptr)
	{
		SelectLayer(mActiveNodeID->GetLayer());
	}
	mActiveNodeID = pNode;
	if (mActiveNodeID)
	{
		SetNodeEditable(mActiveNodeID, true);
		mActiveLayerID = mActiveNodeID->GetLayer();
		if (mActiveLayerID)
			mActiveSceneID = mActiveLayerID->GetScene();
		OnSelectNodeByID(mActiveNodeID->GetID());
	}
}

//! ����ID���ͼԪ�����sceneIdΪ0�����г����в��ң�����ֻ���ҵ�������
IMpNode* CMpSceneManager::GetNodeByID(int nodeID, int layerId, int sceneId)
{
	if (sceneId > 0)
	{
		IMpScene* pScene = GetObjectById(sceneId);
		if (pScene == nullptr) return nullptr;

		if (layerId > 0)
		{
			IMpLayer* pLayer = pScene->GetObjectById(layerId);
			if (pLayer) return pLayer->GetObjectById(nodeID);
			else return nullptr;
		}
		else
		{
			int nLayer = pScene->GetObjectCount();
			for (int i = 0; i < nLayer; i++)
			{
				IMpLayer* pLayer = pScene->GetObjectByIdx(i);
				IMpNode* pNode = pLayer->GetObjectById(nodeID);
				if (pNode) return pNode;
			}
			return nullptr;
		}
	}
	else
	{
		int nScene = GetObjectCount();
		for (int i = 0; i < nScene; i++)
		{
			IMpScene* pScene = GetObjectByIdx(i);

			if (layerId > 0)
			{
				IMpLayer* pLayer = pScene->GetObjectById(layerId);
				if (pLayer)
				{
					IMpNode* pNode = pLayer->GetObjectById(nodeID);
					if (pNode) return pNode;
				}
			}
			else
			{
				int nLayer = pScene->GetObjectCount();
				for (int j = 0; j < nLayer; j++)
				{
					IMpLayer* pLayer = pScene->GetObjectByIdx(j);
					IMpNode* pNode = pLayer->GetObjectById(nodeID);
					if (pNode) return pNode;
				}
			}
		}
	}
	return nullptr;
}

//! �������ƻ�ýڵ�
IMpNode* CMpSceneManager::GetNodeByName(const FString& nodeName)
{
	int nObj = this->GetObjectCount();
	for (int i = 0; i < nObj; i++)
	{
		IMpScene* pObject = GetObjectByIdx(i);
		IMpNode* pNode = pObject->GetNodeByName(nodeName);
		if (pNode) return pNode;
	}
	return nullptr;
}

//! ����ͼ�����ֵõ�ID
int CMpSceneManager::GetNodeIDByName(const FString& mame)
{
	IMpNode* pNode = GetNodeByName(mame);
	if (pNode)
		return pNode->GetID();
	else return 0;
}

//! ����ͼ��ID�õ�����
const FString& CMpSceneManager::GetNodeNameByID(int id)
{
	IMpNode* pNode = GetNodeByID(id);
	if (pNode)
		return pNode->GetName();
	else return EmptyString;
}

//! �ƶ�ͼ�� ����nStepȡֵ��[-N,N]��NΪͼ����������ʵ���ƶ��Ĳ���
int CMpSceneManager::doMoveLayer(IMpLayer* pLayer, int nStep)
{
	if (pLayer == nullptr) return 0;
	IMpScene* pScene = pLayer->GetScene();
	if (pScene == nullptr) return 0;
	int iStep = pScene->MoveObject(pLayer, nStep);
	if (iStep == 0) return 0;
	if (iStep > 0)
	{
		for (int i = 0; i < iStep; i++)
		{
			OnLayerUpdated(pLayer->GetID(), 1);
		}
	}
	else if (iStep < 0)
	{
		for (int i = 0; i < -iStep; i++)
		{
			OnLayerUpdated(pLayer->GetID(), 2);
		}
	}
	return iStep;
}

//! �ƶ�ͼ�� ����nStepȡֵ��[-N,N]��NΪͼ����������ʵ���ƶ��Ĳ���
int CMpSceneManager::MoveLayer(IMpLayer* pLayer, int nStep)
{
	return doMoveLayer(pLayer, nStep);
}

//! ����ͼ��
void CMpSceneManager::MoveLayerUp(IMpLayer* pLayer)
{
	if (pLayer == nullptr) return;
	IMpScene* pScene = pLayer->GetScene();
	if (pScene == nullptr) return;
	MoveLayer(pLayer, 1);
}

//! ����ͼ��
void CMpSceneManager::MoveLayerDown(IMpLayer* pLayer)
{
	if (pLayer == nullptr) return;
	IMpScene* pScene = pLayer->GetScene();
	if (pScene == nullptr) return;
	MoveLayer(pLayer, -1);
}

//! �ö�ͼ��
void CMpSceneManager::MoveLayerTop(IMpLayer* pLayer)
{
	if (pLayer == nullptr) return;
	IMpScene* pScene = pLayer->GetScene();
	if (pScene == nullptr) return;
	int nLayer = pScene->GetObjectCount();
	MoveLayer(pLayer, nLayer);
}

//! �õ�ͼ��
void CMpSceneManager::MoveLayerBottom(IMpLayer* pLayer)
{
	if (pLayer == nullptr) return;
	IMpScene* pScene = pLayer->GetScene();
	if (pScene == nullptr) return;
	int nLayer = pScene->GetObjectCount();
	MoveLayer(pLayer, -nLayer);
}

//! �ƶ�ͼ�� ����nStepȡֵ��[-N,N]��NΪͼ����������ʵ���ƶ��Ĳ���
int CMpSceneManager::doMoveNode(IMpNode* pNode, int nStep)
{
	if (pNode == nullptr) return 0;
	IMpLayer* pLayer = pNode->GetLayer();
	if (pLayer == nullptr) return 0;
	int iStep = pLayer->MoveObject(pNode, nStep);
	if (iStep == 0) return 0;
	if (iStep > 0)
	{
		for (int i = 0; i < iStep; i++)
		{
			OnNodeUpdated(pNode->GetID(), 1);
		}
	}
	else if (iStep < 0)
	{
		for (int i = 0; i < -iStep; i++)
		{
			OnNodeUpdated(pNode->GetID(), 2);
		}
	}
	return iStep;
}

//! �ƶ�ͼ�� ����nStepȡֵ��[-N,N]��NΪͼ����������ʵ���ƶ��Ĳ���
int CMpSceneManager::MoveNode(IMpNode* pNode, int nStep)
{
	return doMoveNode(pNode, nStep);
}

//! ���ƽڵ�
void CMpSceneManager::MoveNodeUp(IMpNode* pNode)
{
	if (pNode == nullptr) return;
	IMpLayer* pLayer = pNode->GetLayer();
	if (pLayer == nullptr) return;
	MoveNode(pNode, 1);
}

//! ���ƽڵ�
void CMpSceneManager::MoveNodeDown(IMpNode* pNode)
{
	if (pNode == nullptr) return;
	IMpLayer* pLayer = pNode->GetLayer();
	if (pLayer == nullptr) return;
	MoveNode(pNode, -1);
}

//! �ö��ڵ�
void CMpSceneManager::MoveNodeTop(IMpNode* pNode)
{
	if (pNode == nullptr) return;
	IMpLayer* pLayer = pNode->GetLayer();
	if (pLayer == nullptr) return;
	int nNode = pLayer->GetObjectCount();
	MoveNode(pNode, nNode);
}

//! �õ׽ڵ�
void CMpSceneManager::MoveNodeBottom(IMpNode* pNode)
{
	if (pNode == nullptr) return;
	IMpLayer* pLayer = pNode->GetLayer();
	if (pLayer == nullptr) return;
	int nNode = pLayer->GetObjectCount();
	MoveNode(pNode, -nNode);
}

//! ȡ��ѡ��
void CMpSceneManager::SelectNothing()
{
	if (mActiveNodeID)
		SetNodeEditable(mActiveNodeID, false);
	mActiveSceneID = nullptr;
	mActiveLayerID = nullptr;
	mActiveNodeID = nullptr;

	ClearSelectedNodes();

	OnSelectNothing();
}


//////////////////////////////////////////////////////////////////////////
//! ���ļ�������ſ�
IMpScene* CMpSceneManager::ImportScenefile(const FString& fileName)
{
	return nullptr;
}

//! �������ſ⵽�ļ�
bool CMpSceneManager::ExportScenefile(IMpScene* pScene, const FString& fileName)
{
	return false;
}
