#pragma once

#include "IMpSceneManager.h"


//!图形，基本的操作对象
class CMpNode : public IMpNode
{
private:
	IMpLayer* Layer;
	IMpGraphics* Graphics;
	TArray<IMpNodeEventListener*> EventListeners;

	bool bShow;
public:

	CMpNode(IMpGraphics* pGraphics, int id, const FString& name)
	{
		Graphics = pGraphics;
		ObjectID = id;
		SetName(name);
		SetVisiable(true);
		SetDetectable(true);
		SetEditable(false);
		Layer = nullptr;
		bShow = true;
	}

	~CMpNode()
	{
		ClearEventListeners();
	}
	//////////////////////////////////////////////////////////////////////////

	//! 挂载图层
	virtual void AttachLayer(IMpLayer* pLayer)
	{
		Layer = pLayer;
	}

	//! 获得图层
	virtual IMpLayer* GetLayer()
	{
		return Layer;
	}


	//////////////////////////////////////////////////////////////////////////
	//! 获得图形
	virtual IMpGraphics* GetGraphics() { return Graphics; }

	//////////////////////////////////////////////////////////////////////////

	//! 注册事件监听
	virtual void RegisterEventListener(IMpNodeEventListener* pEventListener)
	{
		EventListeners.Add(pEventListener);
	}

	//! 解除事件监听
	virtual void UnRegisterEventListener(IMpNodeEventListener* pEventListener)
	{
		EventListeners.Remove(pEventListener);
	}

	void ClearEventListeners()
	{
		EventListeners.Empty();
	}


	//! 当属性发生变化需要更新时
	virtual void OnAttrChanged(int reason = 0)
	{
		for (int i = 0; i < EventListeners.Num(); i++)
		{
			EventListeners[i]->OnAttrChanged(this, reason);
		}
	}

	//! 当图形发生变化需要更新时
	virtual void OnGraphicsUpdated(int reason = 0)
	{
		for (int i = 0; i < EventListeners.Num(); i++)
		{
			EventListeners[i]->OnGraphicsUpdated(this, reason);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	////! 准备内容
	//virtual void PrepareViewContent(IMpViewDrawContext* pView);

};


//!标绘场景图层
class CMpLayer : public IMpLayer
{
	IMpScene* mScene;
public:
	CMpLayer(int id, const FString& layerName)
		: mScene(nullptr)
	{
		ObjectID = id;
		SetName(layerName);
	}

	~CMpLayer()
	{
		ClearObjects();
	}

	//! 挂载图层
	virtual void AttachScene(IMpScene* pScene)
	{
		mScene = pScene;
	}

	//! 获得归属的场景
	virtual IMpScene* GetScene()
	{
		return mScene;
	}

	//! 通过ID获得节点
	virtual IMpNode* GetObjectById(int id)
	{
		int nObj = this->GetObjectCount();
		for (int i = 0; i < nObj; i++)
		{
			IMpNode* pNode = GetObjectByIdx(i);
			if (pNode->GetID() == id)
			{
				return pNode;
			}
		}

		return nullptr;
	}

	//! 根据名称获得节点
	virtual IMpNode* GetNodeByName(const FString& nodeName)
	{
		int nObj = this->GetObjectCount();
		for (int i = 0; i < nObj; i++)
		{
			IMpNode* pNode = GetObjectByIdx(i);
			if (pNode->GetName() == nodeName)
			{
				return pNode;
			}
		}
		return nullptr;
	}
};

//!标绘场景、方案
class CMpScene : public IMpScene
{
	FString mSaveFilePath;
public:
	CMpScene(int id, const FString& sceneName = "")
	{
		ObjectID = id;
		SetName(sceneName);
	}

	~CMpScene()
	{
		ClearObjects();
	}

	//! 根据ID获得场景
	virtual IMpLayer* GetObjectById(int id)
	{
		int nObj = this->GetObjectCount();
		for (int i = 0; i < nObj; i++)
		{
			IMpLayer* pObject = GetObjectByIdx(i);
			if (pObject->GetID() == id)
			{
				return pObject;
			}
		}
		return nullptr;
	}

	//! 根据名称获得图层
	virtual IMpLayer* GetLayerByName(const FString& LayerName)
	{
		int nObj = this->GetObjectCount();
		for (int i = 0; i < nObj; i++)
		{
			IMpLayer* pObject = GetObjectByIdx(i);
			if (pObject->GetName() == LayerName)
			{
				return pObject;
			}
		}
		return nullptr;
	}

	//! 根据名称获得节点
	virtual IMpNode* GetNodeByName(const FString& nodeName)
	{
		int nObj = this->GetObjectCount();
		for (int i = 0; i < nObj; i++)
		{
			IMpLayer* pObject = GetObjectByIdx(i);
			IMpNode* pNode = pObject->GetNodeByName(nodeName);
			if (pNode) return pNode;
		}
		return nullptr;
	}

	//! 设置保存文件名
	virtual void SetSaveFilePath(const FString& filePath)
	{
		mSaveFilePath = filePath;
	}

	//! 获得保存文件名
	virtual const FString& GetSaveFilePath()
	{
		return mSaveFilePath;
	}
};

//! 图形工厂
class CMpSceneFactory : public IMpSceneFactory
{
	IMpSceneManager* mSceneManager;
	int mMaskUID;

	int mSceneUID;
	int mLayerUID;
	int mNodeUID;

	void GMNToMask(int& maskUID, int gid, int mid, int nid)
	{
		maskUID = ((gid & 0x000000ff) << 24) + ((mid & 0x000000ff) << 16) + (nid & 0x0000ffff);
	}

	void MaskToGMN(int maskUID, int& gid, int& mid, int& nid)
	{
		gid = (maskUID & 0xff000000) >> 24;
		mid = (maskUID & 0x00ff0000) >> 16;
		nid = (maskUID & 0x0000ffff);
	}

public:
	CMpSceneFactory(IMpSceneManager* pSceneManager)
		:mSceneManager(pSceneManager)
	{
		mSceneUID = 0;
		mLayerUID = 0;
		mNodeUID = 0;
		mMaskUID = 0;
	}

	//! 设置uid的掩码，用于不同机器创建出来的uid之间确保惟一性
	virtual void ResetUIDMask(int gid = 0, int mid = 0, int nid = 0)
	{
		GMNToMask(mMaskUID, gid, mid, nid);
	}

	//! 创建场景的惟一id
	virtual int CreateSceneUID()
	{
		int id = mMaskUID + (++mSceneUID);
		if (mSceneManager->GetSceneByID(id))
			return CreateSceneUID();
		return id;
	}

	//! 创建图层的惟一id
	virtual int CreateLayerUID()
	{
		int id = mMaskUID + (++mLayerUID);
		if (mSceneManager->GetLayerByID(id))
			return CreateLayerUID();
		return id;
	}

	//! 创建图形的惟一id
	virtual int CreateNodeUID()
	{
		int id = mMaskUID + (++mNodeUID);
		if (mSceneManager->GetNodeByID(id))
			return CreateNodeUID();
		return id;
	}

	//! 创建场景
	virtual IMpScene* CreateScene(int id, const FString& name)
	{
		FString strName(name);
		if (strName == "")
		{
			int gid, mid, nid;
			MaskToGMN(id, gid, mid, nid);

			strName = FString::Printf(TEXT("%d_%d_%d_Scene"), gid, mid, nid);
		}
		return new CMpScene(id, strName);
	}

	//! 创建图层
	virtual IMpLayer* CreateLayer(int id, const FString& name)
	{
		FString strName(name);
		if (strName == "")
		{
			int gid, mid, nid;
			MaskToGMN(id, gid, mid, nid);
			strName = FString::Printf(TEXT("%d_%d_%d_Layer"), gid, mid, nid);
		}
		return new CMpLayer(id, strName);
	}

	//! 创建未知类型图形
	virtual IMpNode* CreateNode(IMpGraphics* pGraphics, int id, const FString& name)
	{
		FString strName(name);
		if (strName == "")
		{
			int gid, mid, nid;
			MaskToGMN(id, gid, mid, nid);
			strName = FString::Printf(TEXT("%d_%d_%d_Node"), gid, mid, nid);
		}

		return new CMpNode(pGraphics, id, strName);
	}
};

//!标绘场景管理器
class CMpSceneManager : public IMpSceneManager,
	public IMpSceneEventListener,
	public IMpNodeEventListener
{
	TArray<IMpNode*> SelectedNodes;
	int SelectNodeIt;

	void ClearSelectedNodes()
	{
		for (auto Node: SelectedNodes)
		{
			Node->SetEditable(false);
		}
		SelectedNodes.Empty();
	}

	IMpNode* GetCurrentSelectNode()
	{
		if (SelectNodeIt >=0 && SelectNodeIt < SelectedNodes.Num())
		{
			return SelectedNodes[SelectNodeIt];
		}
		return nullptr;
	}

	IMpScene* mActiveSceneID;
	IMpLayer* mActiveLayerID;
	IMpNode* mActiveNodeID;

	TArray<IMpSceneEventListener*> EventListeners;
	IMpSceneFactory* mSceneFactory;
public:
	CMpSceneManager()
		: mActiveSceneID(nullptr), mActiveLayerID(nullptr), mActiveNodeID(nullptr)
	{
		mSceneFactory = new CMpSceneFactory(this);//CreateSceneFactory();
		SelectNothing();
	}
	~CMpSceneManager()
	{
		if (mSceneFactory) mSceneFactory->Release();
		ClearEventListeners();
		ClearObjects();
	}

	//! 获得场景工厂
	virtual IMpSceneFactory* GetSceneFactory() { return mSceneFactory; }

	//! 根据ID获得场景
	virtual IMpScene* GetObjectById(int id);

	//! 注册事件监听
	virtual void RegisterEventListener(IMpSceneEventListener* pEventListener)
	{
		EventListeners.Add(pEventListener);
	}

	//! 解除事件监听
	virtual void UnRegisterEventListener(IMpSceneEventListener* pEventListener)
	{
		EventListeners.Remove(pEventListener);
	}

	void ClearEventListeners()
	{
		EventListeners.Empty();
	}

	//! 导入场景文件
	virtual IMpScene* ImportScenefile(const FString& fileName);

	//! 导出场景文件
	virtual bool ExportScenefile(IMpScene* pScene, const FString& fileName);
	//////////////////////////////////////////////////////////////////////////

	//! 创建场景
	virtual IMpScene* CreateScene(int sceneId, const FString& name = "");
	void doAddScene(IMpScene* pScene);

	//! 删除场景
	virtual void DeleteScene(IMpScene* pScene);
	void doDeleteScene(IMpScene* pScene);

	//! 选中场景
	virtual void SelectScene(IMpScene* pScene);

	//! 清空场景
	virtual void ClearScene(IMpScene* pScene);

	//! 设置场景名称
	virtual void SetSceneName(IMpScene* pScene, const FString& name);

	//! 根据ID获得场景
	virtual IMpScene* GetSceneByID(int sceneId);

	//! 根据名称获得场景
	virtual IMpScene* GetSceneByName(const FString& sceneName);

	//! 根据场景名字得到ID
	virtual int GetSceneIDByName(const FString& mame);

	//! 根据场景ID得到名字
	virtual const FString& GetSceneNameByID(int id);

	//////////////////////////////////////////////////////////////////////////

	//! 创建图层
	virtual IMpLayer* CreateLayer(IMpScene* pScene, int layerId, const FString& name = "");
	void doAddLayer(IMpScene* pScene, IMpLayer* pLayer);

	//! 删除图层
	virtual void DeleteLayer(IMpLayer* pLayer);
	void doDeleteLayer(IMpLayer* pLayer);

	//! 设置图层名称
	virtual void SetLayerName(IMpLayer* pLayer, const FString& name);

	//! 设置图层可见
	virtual void SetLayerVisiable(IMpLayer* pLayer, bool visible);

	//! 设置图层可编辑
	virtual void SetLayerEditable(IMpLayer* pLayer, bool editable);

	//! 选中图层
	virtual void SelectLayer(IMpLayer* pLayer);

	//! 清空图层
	virtual void ClearLayer(IMpLayer* pLayer);

	//! 根据ID获得图层，如果sceneId为0从所有场景中查找，否则只查找单个场景
	virtual IMpLayer* GetLayerByID(int layerId, int sceneId = 0);

	//! 根据名称获得图层
	virtual IMpLayer* GetLayerByName(const FString& LayerName);

	//! 根据图层名字得到ID
	virtual int GetLayerIDByName(const FString& mame);

	//! 根据图层ID得到名字
	virtual const FString& GetLayerNameByID(int id);

	//! 移动图层 其中nStep取值在[-N,N]，N为图层数，返回实际移动的步数
	virtual int MoveLayer(IMpLayer* pLayer, int nStep);
	int doMoveLayer(IMpLayer* pLayer, int nStep);

	//! 上移图层
	virtual void MoveLayerUp(IMpLayer* pLayer);

	//! 下移图层
	virtual void MoveLayerDown(IMpLayer* pLayer);

	//! 置顶图层
	virtual void MoveLayerTop(IMpLayer* pLayer);

	//! 置底图层
	virtual void MoveLayerBottom(IMpLayer* pLayer);

	//////////////////////////////////////////////////////////////////////////

	//! 添加图形
	virtual void AddNode(IMpLayer* pLayer, IMpNode* pNode);
	void doAddNode(IMpLayer* pLayer, IMpNode* pNode);

	//! 删除图形
	virtual void DeleteNode(IMpNode* pNode);
	void doDeleteNode(IMpNode* pNode);
	//! 删除选中节点
	virtual void DeleteSelectedNodes();
	////! 复制图形
	//virtual IMpNode* copyNode(IMpNode* pNode);

	//! 复制图形，从节点到文本
	virtual FString CopyNode(IMpNode* pNode);

	//! 粘贴图形，从文本到节点
	virtual IMpNode* ParseNode(const FString& pNodeString);

	//! 设置图形名称
	virtual void SetNodeName(IMpNode* pNode, const FString& name);

	//! 设置图形可见
	virtual void SetNodeVisible(IMpNode* pNode, bool visible);

	//! 设置图形可编辑
	virtual void SetNodeEditable(IMpNode* pNode, bool editable);

	//! 选中图形
	virtual void SelectNode(IMpNode* pNode);

	//! 根据ID获得图元，如果sceneId为0从所有场景中查找，否则只查找单个场景
	virtual IMpNode* GetNodeByID(int nodeID, int layerId = 0, int sceneId = 0);

	//! 根据名称获得节点
	virtual IMpNode* GetNodeByName(const FString& nodeName);

	//! 根据节点名字得到ID
	virtual int GetNodeIDByName(const FString& mame);

	//! 根据节点ID得到名字
	virtual const FString& GetNodeNameByID(int id);

	//! 移动图层 其中nStep取值在[-N,N]，N为图层数，返回实际移动的步数
	virtual int MoveNode(IMpNode* pNode, int nStep);
	int doMoveNode(IMpNode* pNode, int nStep);

	//! 上移节点
	virtual void MoveNodeUp(IMpNode* pNode);

	//! 下移节点
	virtual void MoveNodeDown(IMpNode* pNode);

	//! 置顶节点
	virtual void MoveNodeTop(IMpNode* pNode);

	//! 置底节点
	virtual void MoveNodeBottom(IMpNode* pNode);

	//////////////////////////////////////////////////////////////////////////

	//! 获得当前激活的场景ID
	virtual IMpScene* GetActiveScene() { return mActiveSceneID; }

	//! 获得当前激活的图层ID
	virtual IMpLayer* GetActiveLayer() { return mActiveLayerID; }

	//! 获得当前激活的图形ID
	virtual IMpNode* GetActiveNode() { return mActiveNodeID; }

	//! 取消选中
	virtual void SelectNothing();

	//////////////////////////////////////////////////////////////////////////
	//! 获得多选节点数量
	virtual int GetSelectedNodeCount() { return SelectedNodes.Num(); }

	//! 获得第一个选中节点，如果无选中对象，返回nullptr
	virtual IMpNode* GetFirstSelectedNode()
	{
		SelectNodeIt = 0;
		return GetCurrentSelectNode();
	}

	//! 获得下一个选中节点，如果当前为最后一个对象，返回nullptr
	virtual IMpNode* GetNextSelectedNode()
	{
		SelectNodeIt++;
		return GetCurrentSelectNode();
	}

	//! 获得上一个选中节点，如果当前为第一个对象，返回nullptr
	virtual IMpNode* GetPrevSelectedNode()
	{
		SelectNodeIt--;
		return GetCurrentSelectNode();
	}

	//! 选中节点扩展接口，为了实现多选节点
	virtual bool SelectNodeEx(IMpNode* pNode, bool bSelect = true);

	//! 选中图形
	void doSelectNode(IMpNode* pNode);

	//////////////////////////////////////////////////////////////////////////

	//! 创建场景事件
	virtual void OnCreateScene(int sceneId)
	{
		for (int i = 0; i < EventListeners.Num(); i++)
		{
			EventListeners[i]->OnCreateScene(sceneId);
		}
	}

	//! 删除场景事件
	virtual void OnDeleteScene(int sceneId)
	{
		for (int i = 0; i < EventListeners.Num(); i++)
		{
			EventListeners[i]->OnDeleteScene(sceneId);
		}
	}

	//! 选中场景事件
	virtual void OnSelectScene(int sceneId)
	{
		for (int i = 0; i < EventListeners.Num(); i++)
		{
			EventListeners[i]->OnSelectScene(sceneId);
		}
	}

	//! 清空场景事件
	virtual void OnClearScene(int sceneId)
	{
		for (int i = 0; i < EventListeners.Num(); i++)
		{
			EventListeners[i]->OnClearScene(sceneId);
		}
	}

	//! 创建图层事件
	virtual void OnAddLayer(int sceneId, int layerId)
	{
		for (int i = 0; i < EventListeners.Num(); i++)
		{
			EventListeners[i]->OnAddLayer(sceneId, layerId);
		}
	}

	//! 删除图层事件
	virtual void OnDeleteLayer(int layerId)
	{
		for (int i = 0; i < EventListeners.Num(); i++)
		{
			EventListeners[i]->OnDeleteLayer(layerId);
		}
	}

	//! 设置图层可见性事件
	virtual void OnSetLayerVisiable(int id, bool visible)
	{
		for (int i = 0; i < EventListeners.Num(); i++)
		{
			EventListeners[i]->OnSetLayerVisiable(id, visible);
		}
	}

	//! 设置图层可编辑事件
	virtual void OnSetLayerEditable(int id, bool editable)
	{
		for (int i = 0; i < EventListeners.Num(); i++)
		{
			EventListeners[i]->OnSetLayerEditable(id, editable);
		}
	}

	//! 选中图层事件
	virtual void OnSelectLayer(int layerId)
	{
		for (int i = 0; i < EventListeners.Num(); i++)
		{
			EventListeners[i]->OnSelectLayer(layerId);
		}
	}

	//! 清空图层事件
	virtual void OnClearLayer(int layerId)
	{
		for (int i = 0; i < EventListeners.Num(); i++)
		{
			EventListeners[i]->OnClearLayer(layerId);
		}
	}

	//! 设置图形可见性事件
	virtual void OnSetNodeVisible(int id, bool visible)
	{
		for (int i = 0; i < EventListeners.Num(); i++)
		{
			EventListeners[i]->OnSetNodeVisible(id, visible);
		}
	}

	//! 设置图形可编辑事件
	virtual void OnSetNodeEditable(int id, bool editable)
	{
		for (int i = 0; i < EventListeners.Num(); i++)
		{
			EventListeners[i]->OnSetNodeEditable(id, editable);
		}
	}

	//! 选中图形事件
	virtual void OnSelectNodeByID(int id)
	{
		for (int i = 0; i < EventListeners.Num(); i++)
		{
			EventListeners[i]->OnSelectNodeByID(id);
		}
	}

	//! 创建图形事件
	virtual void OnAddNode(int layerId, int id)
	{
		for (int i = 0; i < EventListeners.Num(); i++)
		{
			EventListeners[i]->OnAddNode(layerId, id);
		}
	}

	//! 删除图形事件
	virtual void OnDeleteNode(int id)
	{
		for (int i = 0; i < EventListeners.Num(); i++)
		{
			EventListeners[i]->OnDeleteNode(id);
		}
	}

	//! 清空选中事件
	virtual void OnSelectNothing()
	{
		for (int i = 0; i < EventListeners.Num(); i++)
		{
			EventListeners[i]->OnSelectNothing();
		}
	}

	//! 场景更新
	virtual void OnSceneUpdated(int id, int reason)
	{
		for (int i = 0; i < EventListeners.Num(); i++)
		{
			EventListeners[i]->OnSceneUpdated(id, reason);
		}
	}

	//! 图层更新
	virtual void OnLayerUpdated(int id, int reason)
	{
		for (int i = 0; i < EventListeners.Num(); i++)
		{
			EventListeners[i]->OnLayerUpdated(id, reason);
		}
	}

	//! 图形更新
	virtual void OnNodeUpdated(int id, int reason)
	{
		for (int i = 0; i < EventListeners.Num(); i++)
		{
			EventListeners[i]->OnNodeUpdated(id, reason);
		}
	}

	//! 当节点属性发生变化需要更新时
	virtual void OnNodeAttrChanged(int id, int reason = 0)
	{
		for (int i = 0; i < EventListeners.Num(); i++)
		{
			EventListeners[i]->OnNodeAttrChanged(id, reason);
		}
	}

	//! 当节点图形发生变化需要更新时
	virtual void OnNodeGraphicsUpdated(int id, int reason = 0)
	{
		for (int i = 0; i < EventListeners.Num(); i++)
		{
			EventListeners[i]->OnNodeGraphicsUpdated(id, reason);
		}
	}

	//! 当属性发生变化需要更新时
	virtual void OnAttrChanged(IMpNode* pNode, int reason = 0)
	{
		OnNodeAttrChanged(pNode->GetID(), reason);
	}

	//! 当图形发生变化需要更新时
	virtual void OnGraphicsUpdated(IMpNode* pNode, int reason = 0)
	{
		OnNodeGraphicsUpdated(pNode->GetID(), reason);
	}

};

