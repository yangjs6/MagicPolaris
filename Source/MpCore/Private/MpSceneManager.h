#pragma once

#include "IMpSceneManager.h"


//!ͼ�Σ������Ĳ�������
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

	//! ����ͼ��
	virtual void AttachLayer(IMpLayer* pLayer)
	{
		Layer = pLayer;
	}

	//! ���ͼ��
	virtual IMpLayer* GetLayer()
	{
		return Layer;
	}


	//////////////////////////////////////////////////////////////////////////
	//! ���ͼ��
	virtual IMpGraphics* GetGraphics() { return Graphics; }

	//////////////////////////////////////////////////////////////////////////

	//! ע���¼�����
	virtual void RegisterEventListener(IMpNodeEventListener* pEventListener)
	{
		EventListeners.Add(pEventListener);
	}

	//! ����¼�����
	virtual void UnRegisterEventListener(IMpNodeEventListener* pEventListener)
	{
		EventListeners.Remove(pEventListener);
	}

	void ClearEventListeners()
	{
		EventListeners.Empty();
	}


	//! �����Է����仯��Ҫ����ʱ
	virtual void OnAttrChanged(int reason = 0)
	{
		for (int i = 0; i < EventListeners.Num(); i++)
		{
			EventListeners[i]->OnAttrChanged(this, reason);
		}
	}

	//! ��ͼ�η����仯��Ҫ����ʱ
	virtual void OnGraphicsUpdated(int reason = 0)
	{
		for (int i = 0; i < EventListeners.Num(); i++)
		{
			EventListeners[i]->OnGraphicsUpdated(this, reason);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	////! ׼������
	//virtual void PrepareViewContent(IMpViewDrawContext* pView);

};


//!��泡��ͼ��
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

	//! ����ͼ��
	virtual void AttachScene(IMpScene* pScene)
	{
		mScene = pScene;
	}

	//! ��ù����ĳ���
	virtual IMpScene* GetScene()
	{
		return mScene;
	}

	//! ͨ��ID��ýڵ�
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

	//! �������ƻ�ýڵ�
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

//!��泡��������
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

	//! ����ID��ó���
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

	//! �������ƻ��ͼ��
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

	//! �������ƻ�ýڵ�
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

	//! ���ñ����ļ���
	virtual void SetSaveFilePath(const FString& filePath)
	{
		mSaveFilePath = filePath;
	}

	//! ��ñ����ļ���
	virtual const FString& GetSaveFilePath()
	{
		return mSaveFilePath;
	}
};

//! ͼ�ι���
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

	//! ����uid�����룬���ڲ�ͬ��������������uid֮��ȷ��Ωһ��
	virtual void ResetUIDMask(int gid = 0, int mid = 0, int nid = 0)
	{
		GMNToMask(mMaskUID, gid, mid, nid);
	}

	//! ����������Ωһid
	virtual int CreateSceneUID()
	{
		int id = mMaskUID + (++mSceneUID);
		if (mSceneManager->GetSceneByID(id))
			return CreateSceneUID();
		return id;
	}

	//! ����ͼ���Ωһid
	virtual int CreateLayerUID()
	{
		int id = mMaskUID + (++mLayerUID);
		if (mSceneManager->GetLayerByID(id))
			return CreateLayerUID();
		return id;
	}

	//! ����ͼ�ε�Ωһid
	virtual int CreateNodeUID()
	{
		int id = mMaskUID + (++mNodeUID);
		if (mSceneManager->GetNodeByID(id))
			return CreateNodeUID();
		return id;
	}

	//! ��������
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

	//! ����ͼ��
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

	//! ����δ֪����ͼ��
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

//!��泡��������
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

	//! ��ó�������
	virtual IMpSceneFactory* GetSceneFactory() { return mSceneFactory; }

	//! ����ID��ó���
	virtual IMpScene* GetObjectById(int id);

	//! ע���¼�����
	virtual void RegisterEventListener(IMpSceneEventListener* pEventListener)
	{
		EventListeners.Add(pEventListener);
	}

	//! ����¼�����
	virtual void UnRegisterEventListener(IMpSceneEventListener* pEventListener)
	{
		EventListeners.Remove(pEventListener);
	}

	void ClearEventListeners()
	{
		EventListeners.Empty();
	}

	//! ���볡���ļ�
	virtual IMpScene* ImportScenefile(const FString& fileName);

	//! ���������ļ�
	virtual bool ExportScenefile(IMpScene* pScene, const FString& fileName);
	//////////////////////////////////////////////////////////////////////////

	//! ��������
	virtual IMpScene* CreateScene(int sceneId, const FString& name = "");
	void doAddScene(IMpScene* pScene);

	//! ɾ������
	virtual void DeleteScene(IMpScene* pScene);
	void doDeleteScene(IMpScene* pScene);

	//! ѡ�г���
	virtual void SelectScene(IMpScene* pScene);

	//! ��ճ���
	virtual void ClearScene(IMpScene* pScene);

	//! ���ó�������
	virtual void SetSceneName(IMpScene* pScene, const FString& name);

	//! ����ID��ó���
	virtual IMpScene* GetSceneByID(int sceneId);

	//! �������ƻ�ó���
	virtual IMpScene* GetSceneByName(const FString& sceneName);

	//! ���ݳ������ֵõ�ID
	virtual int GetSceneIDByName(const FString& mame);

	//! ���ݳ���ID�õ�����
	virtual const FString& GetSceneNameByID(int id);

	//////////////////////////////////////////////////////////////////////////

	//! ����ͼ��
	virtual IMpLayer* CreateLayer(IMpScene* pScene, int layerId, const FString& name = "");
	void doAddLayer(IMpScene* pScene, IMpLayer* pLayer);

	//! ɾ��ͼ��
	virtual void DeleteLayer(IMpLayer* pLayer);
	void doDeleteLayer(IMpLayer* pLayer);

	//! ����ͼ������
	virtual void SetLayerName(IMpLayer* pLayer, const FString& name);

	//! ����ͼ��ɼ�
	virtual void SetLayerVisiable(IMpLayer* pLayer, bool visible);

	//! ����ͼ��ɱ༭
	virtual void SetLayerEditable(IMpLayer* pLayer, bool editable);

	//! ѡ��ͼ��
	virtual void SelectLayer(IMpLayer* pLayer);

	//! ���ͼ��
	virtual void ClearLayer(IMpLayer* pLayer);

	//! ����ID���ͼ�㣬���sceneIdΪ0�����г����в��ң�����ֻ���ҵ�������
	virtual IMpLayer* GetLayerByID(int layerId, int sceneId = 0);

	//! �������ƻ��ͼ��
	virtual IMpLayer* GetLayerByName(const FString& LayerName);

	//! ����ͼ�����ֵõ�ID
	virtual int GetLayerIDByName(const FString& mame);

	//! ����ͼ��ID�õ�����
	virtual const FString& GetLayerNameByID(int id);

	//! �ƶ�ͼ�� ����nStepȡֵ��[-N,N]��NΪͼ����������ʵ���ƶ��Ĳ���
	virtual int MoveLayer(IMpLayer* pLayer, int nStep);
	int doMoveLayer(IMpLayer* pLayer, int nStep);

	//! ����ͼ��
	virtual void MoveLayerUp(IMpLayer* pLayer);

	//! ����ͼ��
	virtual void MoveLayerDown(IMpLayer* pLayer);

	//! �ö�ͼ��
	virtual void MoveLayerTop(IMpLayer* pLayer);

	//! �õ�ͼ��
	virtual void MoveLayerBottom(IMpLayer* pLayer);

	//////////////////////////////////////////////////////////////////////////

	//! ���ͼ��
	virtual void AddNode(IMpLayer* pLayer, IMpNode* pNode);
	void doAddNode(IMpLayer* pLayer, IMpNode* pNode);

	//! ɾ��ͼ��
	virtual void DeleteNode(IMpNode* pNode);
	void doDeleteNode(IMpNode* pNode);
	//! ɾ��ѡ�нڵ�
	virtual void DeleteSelectedNodes();
	////! ����ͼ��
	//virtual IMpNode* copyNode(IMpNode* pNode);

	//! ����ͼ�Σ��ӽڵ㵽�ı�
	virtual FString CopyNode(IMpNode* pNode);

	//! ճ��ͼ�Σ����ı����ڵ�
	virtual IMpNode* ParseNode(const FString& pNodeString);

	//! ����ͼ������
	virtual void SetNodeName(IMpNode* pNode, const FString& name);

	//! ����ͼ�οɼ�
	virtual void SetNodeVisible(IMpNode* pNode, bool visible);

	//! ����ͼ�οɱ༭
	virtual void SetNodeEditable(IMpNode* pNode, bool editable);

	//! ѡ��ͼ��
	virtual void SelectNode(IMpNode* pNode);

	//! ����ID���ͼԪ�����sceneIdΪ0�����г����в��ң�����ֻ���ҵ�������
	virtual IMpNode* GetNodeByID(int nodeID, int layerId = 0, int sceneId = 0);

	//! �������ƻ�ýڵ�
	virtual IMpNode* GetNodeByName(const FString& nodeName);

	//! ���ݽڵ����ֵõ�ID
	virtual int GetNodeIDByName(const FString& mame);

	//! ���ݽڵ�ID�õ�����
	virtual const FString& GetNodeNameByID(int id);

	//! �ƶ�ͼ�� ����nStepȡֵ��[-N,N]��NΪͼ����������ʵ���ƶ��Ĳ���
	virtual int MoveNode(IMpNode* pNode, int nStep);
	int doMoveNode(IMpNode* pNode, int nStep);

	//! ���ƽڵ�
	virtual void MoveNodeUp(IMpNode* pNode);

	//! ���ƽڵ�
	virtual void MoveNodeDown(IMpNode* pNode);

	//! �ö��ڵ�
	virtual void MoveNodeTop(IMpNode* pNode);

	//! �õ׽ڵ�
	virtual void MoveNodeBottom(IMpNode* pNode);

	//////////////////////////////////////////////////////////////////////////

	//! ��õ�ǰ����ĳ���ID
	virtual IMpScene* GetActiveScene() { return mActiveSceneID; }

	//! ��õ�ǰ�����ͼ��ID
	virtual IMpLayer* GetActiveLayer() { return mActiveLayerID; }

	//! ��õ�ǰ�����ͼ��ID
	virtual IMpNode* GetActiveNode() { return mActiveNodeID; }

	//! ȡ��ѡ��
	virtual void SelectNothing();

	//////////////////////////////////////////////////////////////////////////
	//! ��ö�ѡ�ڵ�����
	virtual int GetSelectedNodeCount() { return SelectedNodes.Num(); }

	//! ��õ�һ��ѡ�нڵ㣬�����ѡ�ж��󣬷���nullptr
	virtual IMpNode* GetFirstSelectedNode()
	{
		SelectNodeIt = 0;
		return GetCurrentSelectNode();
	}

	//! �����һ��ѡ�нڵ㣬�����ǰΪ���һ�����󣬷���nullptr
	virtual IMpNode* GetNextSelectedNode()
	{
		SelectNodeIt++;
		return GetCurrentSelectNode();
	}

	//! �����һ��ѡ�нڵ㣬�����ǰΪ��һ�����󣬷���nullptr
	virtual IMpNode* GetPrevSelectedNode()
	{
		SelectNodeIt--;
		return GetCurrentSelectNode();
	}

	//! ѡ�нڵ���չ�ӿڣ�Ϊ��ʵ�ֶ�ѡ�ڵ�
	virtual bool SelectNodeEx(IMpNode* pNode, bool bSelect = true);

	//! ѡ��ͼ��
	void doSelectNode(IMpNode* pNode);

	//////////////////////////////////////////////////////////////////////////

	//! ���������¼�
	virtual void OnCreateScene(int sceneId)
	{
		for (int i = 0; i < EventListeners.Num(); i++)
		{
			EventListeners[i]->OnCreateScene(sceneId);
		}
	}

	//! ɾ�������¼�
	virtual void OnDeleteScene(int sceneId)
	{
		for (int i = 0; i < EventListeners.Num(); i++)
		{
			EventListeners[i]->OnDeleteScene(sceneId);
		}
	}

	//! ѡ�г����¼�
	virtual void OnSelectScene(int sceneId)
	{
		for (int i = 0; i < EventListeners.Num(); i++)
		{
			EventListeners[i]->OnSelectScene(sceneId);
		}
	}

	//! ��ճ����¼�
	virtual void OnClearScene(int sceneId)
	{
		for (int i = 0; i < EventListeners.Num(); i++)
		{
			EventListeners[i]->OnClearScene(sceneId);
		}
	}

	//! ����ͼ���¼�
	virtual void OnAddLayer(int sceneId, int layerId)
	{
		for (int i = 0; i < EventListeners.Num(); i++)
		{
			EventListeners[i]->OnAddLayer(sceneId, layerId);
		}
	}

	//! ɾ��ͼ���¼�
	virtual void OnDeleteLayer(int layerId)
	{
		for (int i = 0; i < EventListeners.Num(); i++)
		{
			EventListeners[i]->OnDeleteLayer(layerId);
		}
	}

	//! ����ͼ��ɼ����¼�
	virtual void OnSetLayerVisiable(int id, bool visible)
	{
		for (int i = 0; i < EventListeners.Num(); i++)
		{
			EventListeners[i]->OnSetLayerVisiable(id, visible);
		}
	}

	//! ����ͼ��ɱ༭�¼�
	virtual void OnSetLayerEditable(int id, bool editable)
	{
		for (int i = 0; i < EventListeners.Num(); i++)
		{
			EventListeners[i]->OnSetLayerEditable(id, editable);
		}
	}

	//! ѡ��ͼ���¼�
	virtual void OnSelectLayer(int layerId)
	{
		for (int i = 0; i < EventListeners.Num(); i++)
		{
			EventListeners[i]->OnSelectLayer(layerId);
		}
	}

	//! ���ͼ���¼�
	virtual void OnClearLayer(int layerId)
	{
		for (int i = 0; i < EventListeners.Num(); i++)
		{
			EventListeners[i]->OnClearLayer(layerId);
		}
	}

	//! ����ͼ�οɼ����¼�
	virtual void OnSetNodeVisible(int id, bool visible)
	{
		for (int i = 0; i < EventListeners.Num(); i++)
		{
			EventListeners[i]->OnSetNodeVisible(id, visible);
		}
	}

	//! ����ͼ�οɱ༭�¼�
	virtual void OnSetNodeEditable(int id, bool editable)
	{
		for (int i = 0; i < EventListeners.Num(); i++)
		{
			EventListeners[i]->OnSetNodeEditable(id, editable);
		}
	}

	//! ѡ��ͼ���¼�
	virtual void OnSelectNodeByID(int id)
	{
		for (int i = 0; i < EventListeners.Num(); i++)
		{
			EventListeners[i]->OnSelectNodeByID(id);
		}
	}

	//! ����ͼ���¼�
	virtual void OnAddNode(int layerId, int id)
	{
		for (int i = 0; i < EventListeners.Num(); i++)
		{
			EventListeners[i]->OnAddNode(layerId, id);
		}
	}

	//! ɾ��ͼ���¼�
	virtual void OnDeleteNode(int id)
	{
		for (int i = 0; i < EventListeners.Num(); i++)
		{
			EventListeners[i]->OnDeleteNode(id);
		}
	}

	//! ���ѡ���¼�
	virtual void OnSelectNothing()
	{
		for (int i = 0; i < EventListeners.Num(); i++)
		{
			EventListeners[i]->OnSelectNothing();
		}
	}

	//! ��������
	virtual void OnSceneUpdated(int id, int reason)
	{
		for (int i = 0; i < EventListeners.Num(); i++)
		{
			EventListeners[i]->OnSceneUpdated(id, reason);
		}
	}

	//! ͼ�����
	virtual void OnLayerUpdated(int id, int reason)
	{
		for (int i = 0; i < EventListeners.Num(); i++)
		{
			EventListeners[i]->OnLayerUpdated(id, reason);
		}
	}

	//! ͼ�θ���
	virtual void OnNodeUpdated(int id, int reason)
	{
		for (int i = 0; i < EventListeners.Num(); i++)
		{
			EventListeners[i]->OnNodeUpdated(id, reason);
		}
	}

	//! ���ڵ����Է����仯��Ҫ����ʱ
	virtual void OnNodeAttrChanged(int id, int reason = 0)
	{
		for (int i = 0; i < EventListeners.Num(); i++)
		{
			EventListeners[i]->OnNodeAttrChanged(id, reason);
		}
	}

	//! ���ڵ�ͼ�η����仯��Ҫ����ʱ
	virtual void OnNodeGraphicsUpdated(int id, int reason = 0)
	{
		for (int i = 0; i < EventListeners.Num(); i++)
		{
			EventListeners[i]->OnNodeGraphicsUpdated(id, reason);
		}
	}

	//! �����Է����仯��Ҫ����ʱ
	virtual void OnAttrChanged(IMpNode* pNode, int reason = 0)
	{
		OnNodeAttrChanged(pNode->GetID(), reason);
	}

	//! ��ͼ�η����仯��Ҫ����ʱ
	virtual void OnGraphicsUpdated(IMpNode* pNode, int reason = 0)
	{
		OnNodeGraphicsUpdated(pNode->GetID(), reason);
	}

};

