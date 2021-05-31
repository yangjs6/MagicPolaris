#include "MpSceneManager.h"

const FString EmptyString;

//////////////////////////////////////////////////////////////////////////

void CMpSceneManager::doAddScene(IMpScene* pScene)
{
	AddObject(pScene);
	OnCreateScene(pScene->GetID());
	SelectScene(pScene);
}

//! 创建场景
IMpScene* CMpSceneManager::CreateScene(int sceneId, const FString& name)
{
	IMpScene* pScene = this->GetObjectById(sceneId);
	if (pScene) return pScene;

	pScene = mSceneFactory->CreateScene(sceneId, name);

	doAddScene(pScene);

	pScene->Release();
	return pScene;
}

//! 删除场景
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

//! 删除场景
void CMpSceneManager::DeleteScene(IMpScene* pScene)
{
	doDeleteScene(pScene);
}

//! 选中场景
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

//! 清空场景
void CMpSceneManager::ClearScene(IMpScene* pScene)
{
	if (pScene == nullptr)
		return;

	// 遍历删除图层下所有的节点
	while (pScene->GetObjectCount() > 0)
	{
		DeleteLayer(pScene->GetObjectByIdx(0));
	}
	//pScene->ClearObjects();
	OnClearScene(pScene->GetID());

}

//! 设置场景名称
void CMpSceneManager::SetSceneName(IMpScene* pScene, const FString& name)
{
	if (pScene)
	{
		pScene->SetName(name);
		OnSceneUpdated(pScene->GetID(), 1);
	}
}

//! 根据ID获得场景
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

//! 根据ID获得场景
IMpScene* CMpSceneManager::GetSceneByID(int sceneId)
{
	return GetObjectById(sceneId);
}

//! 根据名称获得场景
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

//! 根据场景名字得到ID
int CMpSceneManager::GetSceneIDByName(const FString& mame)
{
	IMpScene* pScene = GetSceneByName(mame);
	if (pScene)
		return pScene->GetID();
	else return 0;
}

//! 根据场景ID得到名字
const FString& CMpSceneManager::GetSceneNameByID(int id)
{
	IMpScene* pScene = GetSceneByID(id);
	if (pScene)
		return pScene->GetName();
	else return EmptyString;
}

//////////////////////////////////////////////////////////////////////////

//! 创建图层
void CMpSceneManager::doAddLayer(IMpScene* pScene, IMpLayer* pLayer)
{
	pLayer->AttachScene(pScene);
	pScene->AddObject(pLayer);
	OnAddLayer(pScene->GetID(), pLayer->GetID());
	//SelectLayer(pLayer);
}

//! 创建图层
IMpLayer* CMpSceneManager::CreateLayer(IMpScene* pScene, int layerId, const FString& name)
{
	IMpLayer* pLayer = pScene->GetObjectById(layerId);
	if (pLayer) return pLayer;

	pLayer = mSceneFactory->CreateLayer(layerId, name);

	doAddLayer(pScene, pLayer);

	pLayer->Release();
	return pLayer;
}

//! 删除图层
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

//! 删除图层
void CMpSceneManager::DeleteLayer(IMpLayer* pLayer)
{
	doDeleteLayer(pLayer);
}

//! 设置图层名称
void CMpSceneManager::SetLayerName(IMpLayer* pLayer, const FString& name)
{
	pLayer->SetName(name);
	OnLayerUpdated(pLayer->GetID(), 0);
}

//! 设置图层可见
void CMpSceneManager::SetLayerVisiable(IMpLayer* pLayer, bool visible)
{
	pLayer->SetVisiable(visible);
	OnSetLayerVisiable(pLayer->GetID(), visible);
}

//! 设置图层可编辑
void CMpSceneManager::SetLayerEditable(IMpLayer* pLayer, bool editable)
{
	pLayer->SetEditable(editable);
	OnSetLayerEditable(pLayer->GetID(), editable);
}

//! 选中图层
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

//! 清空图层
void CMpSceneManager::ClearLayer(IMpLayer* pLayer)
{
	if (pLayer == nullptr) return;

	// 遍历删除图层下所有的节点
	while (pLayer->GetObjectCount() > 0)
	{
		DeleteNode(pLayer->GetObjectByIdx(0));
	}
	//pLayer->ClearObjects();
	OnClearLayer(pLayer->GetID());

}

//! 根据ID获得图层，如果sceneId为0从所有场景中查找，否则只查找单个场景
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

//! 根据名称获得图层
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

//! 根据图层名字得到ID
int CMpSceneManager::GetLayerIDByName(const FString& mame)
{
	IMpLayer* pLayer = GetLayerByName(mame);
	if (pLayer)
		return pLayer->GetID();
	else return 0;
}

//! 根据图层ID得到名字
const FString& CMpSceneManager::GetLayerNameByID(int id)
{
	IMpLayer* pLayer = GetLayerByID(id);
	if (pLayer)
		return pLayer->GetName();
	else return EmptyString;
}
//////////////////////////////////////////////////////////////////////////

//! 添加图形
void CMpSceneManager::doAddNode(IMpLayer* pLayer, IMpNode* pNode)
{
	IMpLayer* pLayerOld = pNode->GetLayer();
	if (pLayerOld == pLayer) return;

	// 先删除老节点
	if (pLayerOld)
	{
		OnDeleteNode(pNode->GetID());
		pLayerOld->RemoveObject(pNode);
	}

	// 添加到新的层中
	if (pLayer)
	{
		pNode->RegisterEventListener(this);
		pNode->AttachLayer(pLayer);
		pLayer->AddObject(pNode);

		OnAddNode(pLayer->GetID(), pNode->GetID());
	}
}

//! 添加图形
void CMpSceneManager::AddNode(IMpLayer* pLayer, IMpNode* pNode)
{
	doAddNode(pLayer, pNode);
}

//! 删除图形
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

//! 删除图形
void CMpSceneManager::DeleteNode(IMpNode* pNode)
{
	doDeleteNode(pNode);
}
void CMpSceneManager::DeleteSelectedNodes()
{
}

//! 复制图形，从节点到文本
FString CMpSceneManager::CopyNode(IMpNode* pNode)
{
	return FString();
}

//! 粘贴图形，从文本到节点
IMpNode* CMpSceneManager::ParseNode(const FString& pNodeString)
{
	return nullptr;
}

//! 设置图形名称
void CMpSceneManager::SetNodeName(IMpNode* pNode, const FString& name)
{
	pNode->SetName(name);
	OnNodeUpdated(pNode->GetID(), 0);
}

//! 设置图形可见
void CMpSceneManager::SetNodeVisible(IMpNode* pNode, bool visible)
{
	if (pNode->GetVisiable() != visible)
	{
		pNode->SetVisiable(visible);
		OnSetNodeVisible(pNode->GetID(), visible);
	}
}

//! 设置图形可编辑
void CMpSceneManager::SetNodeEditable(IMpNode* pNode, bool editable)
{
	if (pNode->GetEditable() != editable)
	{
		pNode->SetEditable(editable);
		OnSetNodeVisible(pNode->GetID(), editable);
	}
}

//! 选中图形
void CMpSceneManager::SelectNode(IMpNode* pNode)
{
	ClearSelectedNodes();
	SelectNodeEx(pNode);
}

//! 选中节点扩展接口，为了实现多选节点
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

//! 选中图形
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

//! 根据ID获得图元，如果sceneId为0从所有场景中查找，否则只查找单个场景
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

//! 根据名称获得节点
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

//! 根据图层名字得到ID
int CMpSceneManager::GetNodeIDByName(const FString& mame)
{
	IMpNode* pNode = GetNodeByName(mame);
	if (pNode)
		return pNode->GetID();
	else return 0;
}

//! 根据图层ID得到名字
const FString& CMpSceneManager::GetNodeNameByID(int id)
{
	IMpNode* pNode = GetNodeByID(id);
	if (pNode)
		return pNode->GetName();
	else return EmptyString;
}

//! 移动图层 其中nStep取值在[-N,N]，N为图层数，返回实际移动的步数
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

//! 移动图层 其中nStep取值在[-N,N]，N为图层数，返回实际移动的步数
int CMpSceneManager::MoveLayer(IMpLayer* pLayer, int nStep)
{
	return doMoveLayer(pLayer, nStep);
}

//! 上移图层
void CMpSceneManager::MoveLayerUp(IMpLayer* pLayer)
{
	if (pLayer == nullptr) return;
	IMpScene* pScene = pLayer->GetScene();
	if (pScene == nullptr) return;
	MoveLayer(pLayer, 1);
}

//! 下移图层
void CMpSceneManager::MoveLayerDown(IMpLayer* pLayer)
{
	if (pLayer == nullptr) return;
	IMpScene* pScene = pLayer->GetScene();
	if (pScene == nullptr) return;
	MoveLayer(pLayer, -1);
}

//! 置顶图层
void CMpSceneManager::MoveLayerTop(IMpLayer* pLayer)
{
	if (pLayer == nullptr) return;
	IMpScene* pScene = pLayer->GetScene();
	if (pScene == nullptr) return;
	int nLayer = pScene->GetObjectCount();
	MoveLayer(pLayer, nLayer);
}

//! 置底图层
void CMpSceneManager::MoveLayerBottom(IMpLayer* pLayer)
{
	if (pLayer == nullptr) return;
	IMpScene* pScene = pLayer->GetScene();
	if (pScene == nullptr) return;
	int nLayer = pScene->GetObjectCount();
	MoveLayer(pLayer, -nLayer);
}

//! 移动图层 其中nStep取值在[-N,N]，N为图层数，返回实际移动的步数
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

//! 移动图层 其中nStep取值在[-N,N]，N为图层数，返回实际移动的步数
int CMpSceneManager::MoveNode(IMpNode* pNode, int nStep)
{
	return doMoveNode(pNode, nStep);
}

//! 上移节点
void CMpSceneManager::MoveNodeUp(IMpNode* pNode)
{
	if (pNode == nullptr) return;
	IMpLayer* pLayer = pNode->GetLayer();
	if (pLayer == nullptr) return;
	MoveNode(pNode, 1);
}

//! 下移节点
void CMpSceneManager::MoveNodeDown(IMpNode* pNode)
{
	if (pNode == nullptr) return;
	IMpLayer* pLayer = pNode->GetLayer();
	if (pLayer == nullptr) return;
	MoveNode(pNode, -1);
}

//! 置顶节点
void CMpSceneManager::MoveNodeTop(IMpNode* pNode)
{
	if (pNode == nullptr) return;
	IMpLayer* pLayer = pNode->GetLayer();
	if (pLayer == nullptr) return;
	int nNode = pLayer->GetObjectCount();
	MoveNode(pNode, nNode);
}

//! 置底节点
void CMpSceneManager::MoveNodeBottom(IMpNode* pNode)
{
	if (pNode == nullptr) return;
	IMpLayer* pLayer = pNode->GetLayer();
	if (pLayer == nullptr) return;
	int nNode = pLayer->GetObjectCount();
	MoveNode(pNode, -nNode);
}

//! 取消选中
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
//! 从文件导入符号库
IMpScene* CMpSceneManager::ImportScenefile(const FString& fileName)
{
	return nullptr;
}

//! 导出符号库到文件
bool CMpSceneManager::ExportScenefile(IMpScene* pScene, const FString& fileName)
{
	return false;
}
