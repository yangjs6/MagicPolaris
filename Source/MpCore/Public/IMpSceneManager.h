#pragma once

#include "IMpUnknown.h"
#include "IMpObjectContainer.h"

class IMpGraphics;
class IMpNode;
class IMpLayer;
class IMpScene;
class IMpSceneFactory;
class IMpSceneManager;

//! �����������
class IMpSceneObject : public IMpUnknown
{
protected:
	int ObjectID;
	FString ObjectName;
	FString ObjectNameID;
	//! �ɼ���
	bool bVisiable;
	//! �ɼ����
	bool bDetectable;
	//! �ɱ�����
	bool bEditable;

	IMpSceneObject()
		:ObjectID(-1),
		bVisiable(true), 
		bDetectable(true), 
		bEditable(true)
	{

	}

	virtual bool CanDeleteSelf() { return true; }
public:

	//! ���id
	virtual int GetID() { return ObjectID; }

	//! �������
	virtual const FString& GetName() { return ObjectName; }

	//! ��������
	virtual void SetName(const FString& name) { ObjectName = name; }

	//! �������ID
	virtual const FString& GetNameID() { return ObjectNameID; }

	//! ��������ID
	virtual void SetNameID(const FString& name) { ObjectNameID = name; }

	//! ���ÿɼ���
	virtual void SetVisiable(bool visiable) { bVisiable = visiable; }

	//! ���ñ༭��
	virtual void SetEditable(bool editable) { bEditable = editable; }

	//! ���ÿɼ����
	virtual void SetDetectable(bool detectable) { bDetectable = detectable; }

	//! ��ÿɼ���
	virtual bool GetVisiable() { return bVisiable; }

	//! ��ñ༭��
	virtual bool GetEditable() { return bEditable; }

	//! ��ÿɼ����
	virtual bool GetDetectable() { return bDetectable; }

	////////////////////////////////////////////////////////////////////////////
	// ���л���������
	virtual int Serialize(FString& data) { return 0; }

	// �����л����ṹ��
	virtual int Deserialize(const FString& data) { return 0; }
};

//! �ڵ��¼�����
class IMpNodeEventListener : public IMpUnknown
{
public:
	//! �����Է����仯��Ҫ����ʱ
	virtual void OnAttrChanged(IMpNode* pNode, int reason = 0) = 0;

	//! ��ͼ�η����仯��Ҫ����ʱ
	virtual void OnGraphicsUpdated(IMpNode* pNode, int reason = 0) = 0;
};

//!ͼ�Σ������Ĳ�������
class IMpNode : public IMpSceneObject
{
public:
	//! ����ͼ��
	virtual void AttachLayer(IMpLayer* pLayer) = 0;

	//! ���ͼ��
	virtual IMpLayer* GetLayer() = 0;

	//////////////////////////////////////////////////////////////////////////
	//! ���ͼ��
	virtual IMpGraphics* GetGraphics() = 0;

	//////////////////////////////////////////////////////////////////////////
	//! ע���¼�����
	virtual void RegisterEventListener(IMpNodeEventListener* pEventListener) = 0;

	//! ����¼�����
	virtual void UnRegisterEventListener(IMpNodeEventListener* pEventListener) = 0;

	//! �����Է����仯��Ҫ����ʱ
	virtual void OnAttrChanged(int reason = 0) = 0;

	//! ��ͼ�η����仯��Ҫ����ʱ
	virtual void OnGraphicsUpdated(int reason = 0) = 0;
	////////////////////////////////////////////////////////////////////////////
	////! ׼������
	//virtual void PrepareViewContent(IMpViewDrawContext* pView) = 0;
};

//!��泡��ͼ��
class IMpLayer : public IMpSceneObject,
	public IMpObjectContainer<IMpNode>
{
public:
	//! ����ͼ��
	virtual void AttachScene(IMpScene* pScene) = 0;

	//! ��ù����ĳ���
	virtual IMpScene* GetScene() = 0;

	//! �������ƻ�ýڵ�
	virtual IMpNode* GetNodeByName(const FString& nodeName) = 0;
};

//!��泡��������
class IMpScene : public IMpSceneObject,
	public IMpObjectContainer<IMpLayer>
{
public:
	//! �������ƻ��ͼ��
	virtual IMpLayer* GetLayerByName(const FString& LayerName) = 0;

	//! �������ƻ�ýڵ�
	virtual IMpNode* GetNodeByName(const FString& nodeName) = 0;

	//! ���ñ����ļ���
	virtual void SetSaveFilePath(const FString& filePath) = 0;

	//! ��ñ����ļ���
	virtual const FString& GetSaveFilePath() = 0;
};

//!��泡�������¼�����
class IMpSceneEventListener : public IMpUnknown
{
public:
	//! ���������¼�
	virtual void OnCreateScene(int sceneId) = 0;

	//! ɾ�������¼�
	virtual void OnDeleteScene(int sceneId) = 0;

	//! ѡ�г����¼�
	virtual void OnSelectScene(int sceneId) = 0;

	//! ��ճ����¼�
	virtual void OnClearScene(int sceneId) = 0;

	//! ����ͼ���¼�
	virtual void OnAddLayer(int sceneId, int layerId) = 0;

	//! ɾ��ͼ���¼�
	virtual void OnDeleteLayer(int layerId) = 0;

	//! ����ͼ��ɼ����¼�
	virtual void OnSetLayerVisiable(int id, bool visible) = 0;

	//! ����ͼ��ɱ༭�¼�
	virtual void OnSetLayerEditable(int id, bool editable) = 0;

	//! ѡ��ͼ���¼�
	virtual void OnSelectLayer(int layerId) = 0;

	//! ���ͼ���¼�
	virtual void OnClearLayer(int layerId) = 0;

	//! ����ͼ���¼�
	virtual void OnAddNode(int layerId, int id) = 0;

	//! ɾ��ͼ���¼�
	virtual void OnDeleteNode(int id) = 0;

	//! ����ͼ�οɼ����¼�
	virtual void OnSetNodeVisible(int id, bool visible) = 0;

	//! ����ͼ�οɱ༭�¼�
	virtual void OnSetNodeEditable(int id, bool editable) = 0;

	//! ѡ��ͼ���¼�
	virtual void OnSelectNodeByID(int id) = 0;

	//! ���ѡ���¼�
	virtual void OnSelectNothing() = 0;

	//! �������� 0 Ϊ�����޸� 1-4Ϊ�ڵ������ƶ���־
	virtual void OnSceneUpdated(int id, int reason) = 0;

	//! ͼ����� 0 Ϊ�����޸� 1-4Ϊ�ڵ������ƶ���־
	virtual void OnLayerUpdated(int id, int reason) = 0;

	//! ͼ�θ��� 0 Ϊ�����޸� 1-4Ϊ�ڵ������ƶ���־
	virtual void OnNodeUpdated(int id, int reason) = 0;

	//! ���ڵ����Է����仯��Ҫ����ʱ
	virtual void OnNodeAttrChanged(int id, int reason = 0) = 0;

	//! ���ڵ�ͼ�η����仯��Ҫ����ʱ
	virtual void OnNodeGraphicsUpdated(int id, int reason = 0) = 0;
};

//!��泡��������
class IMpSceneManager : public IMpUnknown,
	public IMpObjectContainer<IMpScene>
{
public:
	//! ��ó�������
	virtual IMpSceneFactory* GetSceneFactory() = 0;

	//! ע���¼�����
	virtual void RegisterEventListener(IMpSceneEventListener* pEventListener) = 0;

	//! ����¼�����
	virtual void UnRegisterEventListener(IMpSceneEventListener* pEventListener) = 0;

	//! ���볡���ļ�
	virtual IMpScene* ImportScenefile(const FString& fileName) = 0;

	//! ���������ļ�
	virtual bool ExportScenefile(IMpScene* pScene, const FString& fileName) = 0;

	//////////////////////////////////////////////////////////////////////////

	//! ��������
	virtual IMpScene* CreateScene(int sceneId, const FString& name = "") = 0;

	//! ɾ������
	virtual void DeleteScene(IMpScene* pScene) = 0;

	//! ���ó�������
	virtual void SetSceneName(IMpScene* pScene, const FString& name) = 0;

	//! ѡ�г���
	virtual void SelectScene(IMpScene* pScene) = 0;

	//! ��ճ���
	virtual void ClearScene(IMpScene* pScene) = 0;

	//! ����ID��ó���
	virtual IMpScene* GetSceneByID(int sceneId) = 0;

	//! �������ƻ�ó���
	virtual IMpScene* GetSceneByName(const FString& sceneName) = 0;

	//! ���ݳ������ֵõ�ID
	virtual int GetSceneIDByName(const FString& mame) = 0;

	//! ���ݳ���ID�õ�����
	virtual const FString& GetSceneNameByID(int id) = 0;

	//////////////////////////////////////////////////////////////////////////

	//! ����ͼ��
	virtual IMpLayer* CreateLayer(IMpScene* pScene, int layerId, const FString& name = "") = 0;

	//! ɾ��ͼ��
	virtual void DeleteLayer(IMpLayer* pLayer) = 0;

	//! ����ͼ������
	virtual void SetLayerName(IMpLayer* pLayer, const FString& name) = 0;

	//! ����ͼ��ɼ�
	virtual void SetLayerVisiable(IMpLayer* pLayer, bool visible) = 0;

	//! ����ͼ��ɱ༭
	virtual void SetLayerEditable(IMpLayer* pLayer, bool editable) = 0;

	//! ѡ��ͼ��
	virtual void SelectLayer(IMpLayer* pLayer) = 0;

	//! ���ͼ��
	virtual void ClearLayer(IMpLayer* pLayer) = 0;

	//! ����ID���ͼ�㣬���sceneIdΪ0�����г����в��ң�����ֻ���ҵ�������
	virtual IMpLayer* GetLayerByID(int layerId, int sceneId = 0) = 0;

	//! �������ƻ��ͼ��
	virtual IMpLayer* GetLayerByName(const FString& layerName) = 0;

	//! ����ͼ�����ֵõ�ID
	virtual int GetLayerIDByName(const FString& mame) = 0;

	//! ����ͼ��ID�õ�����
	virtual const FString& GetLayerNameByID(int id) = 0;

	//! �ƶ�ͼ�� ����nStepȡֵ��[-N,N]��NΪͼ����������ʵ���ƶ��Ĳ���
	virtual int MoveLayer(IMpLayer* pLayer, int nStep) = 0;

	//! ����ͼ��
	virtual void MoveLayerUp(IMpLayer* pLayer) = 0;

	//! ����ͼ��
	virtual void MoveLayerDown(IMpLayer* pLayer) = 0;

	//! �ö�ͼ��
	virtual void MoveLayerTop(IMpLayer* pLayer) = 0;

	//! �õ�ͼ��
	virtual void MoveLayerBottom(IMpLayer* pLayer) = 0;

	//////////////////////////////////////////////////////////////////////////

	//! ���ͼ��
	virtual void AddNode(IMpLayer* pLayer, IMpNode* pNode) = 0;

	//! ɾ��ͼ��
	virtual void DeleteNode(IMpNode* pNode) = 0;

	//! ɾ������ѡ�нڵ�
	virtual void DeleteSelectedNodes() = 0;

	////! ����ͼ��
	//virtual IMpNode* copyNode(IMpNode* pNode)=0;

	//! ����ͼ�Σ��ӽڵ㵽�ı�
	virtual FString CopyNode(IMpNode* pNode) = 0;

	//! ճ��ͼ�Σ����ı����ڵ�
	virtual IMpNode* ParseNode(const FString& pNodeString) = 0;

	//! ����ͼ������
	virtual void SetNodeName(IMpNode* pNode, const FString& name) = 0;

	//! ����ͼ�οɼ�
	virtual void SetNodeVisible(IMpNode* pNode, bool visible) = 0;

	//! ����ͼ�οɱ༭
	virtual void SetNodeEditable(IMpNode* pNode, bool editable) = 0;

	//! ѡ��ͼ��
	virtual void SelectNode(IMpNode* pNode) = 0;

	//! ����ID��ýڵ㣬���sceneIdΪ0�����г����в��ң�����ֻ���ҵ�������
	virtual IMpNode* GetNodeByID(int nodeID, int layerId = 0, int sceneId = 0) = 0;

	//! �������ƻ�ýڵ�
	virtual IMpNode* GetNodeByName(const FString& nodeName) = 0;

	//! ���ݽڵ����ֵõ�ID
	virtual int GetNodeIDByName(const FString& mame) = 0;

	//! ���ݽڵ�ID�õ�����
	virtual const FString& GetNodeNameByID(int id) = 0;

	//! �ƶ�ͼ�� ����nStepȡֵ��[-N,N]��NΪͼ����������ʵ���ƶ��Ĳ���
	virtual int MoveNode(IMpNode* pNode, int nStep) = 0;

	//! ���ƽڵ�
	virtual void MoveNodeUp(IMpNode* pNode) = 0;

	//! ���ƽڵ�
	virtual void MoveNodeDown(IMpNode* pNode) = 0;

	//! �ö��ڵ�
	virtual void MoveNodeTop(IMpNode* pNode) = 0;

	//! �õ׽ڵ�
	virtual void MoveNodeBottom(IMpNode* pNode) = 0;

	//////////////////////////////////////////////////////////////////////////

	//! ȡ��ѡ��
	virtual void SelectNothing() = 0;

	//! ��õ�ǰ����ĳ���ID
	virtual IMpScene* GetActiveScene() = 0;

	//! ��õ�ǰ�����ͼ��ID
	virtual IMpLayer* GetActiveLayer() = 0;

	//! ��õ�ǰ�����ͼ��ID
	virtual IMpNode* GetActiveNode() = 0;

	//////////////////////////////////////////////////////////////////////////
	//! ��ö�ѡ�ڵ�����
	virtual int GetSelectedNodeCount() = 0;

	//! ��õ�һ��ѡ�нڵ㣬�����ѡ�ж��󣬷���NULL
	virtual IMpNode* GetFirstSelectedNode() = 0;

	//! �����һ��ѡ�нڵ㣬�����ǰΪ���һ�����󣬷���NULL
	virtual IMpNode* GetNextSelectedNode() = 0;

	//! �����һ��ѡ�нڵ㣬�����ǰΪ��һ�����󣬷���NULL
	virtual IMpNode* GetPrevSelectedNode() = 0;

	//! ѡ�нڵ���չ�ӿڣ�Ϊ��ʵ�ֶ�ѡ�ڵ�
	virtual bool SelectNodeEx(IMpNode* pNode, bool bSelect = true) = 0;
};

//! ͼ�ι���
class IMpSceneFactory : public IMpUnknown
{
public:
	//! ����uid�����룬���ڲ�ͬ��������������uid֮��ȷ��Ωһ��
	virtual void ResetUIDMask(int gid = 0, int mid = 0, int nid = 0) = 0;

	//! ����������Ωһid
	virtual int CreateSceneUID() = 0;

	//! ����ͼ���Ωһid
	virtual int CreateLayerUID() = 0;

	//! ����ͼ�ε�Ωһid
	virtual int CreateNodeUID() = 0;

	//! ��������
	virtual IMpScene* CreateScene(int id, const FString& name = "") = 0;

	//! ����ͼ��
	virtual IMpLayer* CreateLayer(int id, const FString& name = "") = 0;

	//! �����ڵ�
	virtual IMpNode* CreateNode(IMpGraphics* pGraphics, int id, const FString& name = "") = 0;

};

class FMpSceneUtils
{
public:
	//��õ�ǰ������������Ϊ��ʱ����һ���µĳ���
	static IMpScene* GetOrCreateScene(IMpSceneManager* pSceneManager, const FString& SceneName)
	{
		if (pSceneManager == nullptr)
		{
			return nullptr;
		}
		IMpScene* pScene = pSceneManager->GetSceneByName(SceneName);
		if (pScene == nullptr)
		{
			int sceneId = pSceneManager->GetSceneFactory()->CreateSceneUID();
			pScene = pSceneManager->CreateScene(sceneId);
			pScene->SetName(SceneName);
		}
		return pScene;
	}

	//��õ�ǰͼ�㣻 ��ͼ��Ϊ��ʱ����һ���µ�ͼ��
	static IMpLayer* GetOrCreateLayer(IMpSceneManager* pSceneManager, IMpScene* pScene, const FString& LayerName)
	{
		if (pScene == nullptr)
		{
			return nullptr;
		}
		IMpLayer* pLayer = pSceneManager->GetLayerByName(LayerName);
		if (pLayer == nullptr)
		{
			int layerId = pSceneManager->GetSceneFactory()->CreateLayerUID();
			pLayer = pSceneManager->CreateLayer(pScene, layerId);
			pLayer->SetName(LayerName);
		}
		return pLayer;
	}

	static IMpNode* CreateNode(IMpGraphics* pGraphics, IMpSceneManager* SceneManager, IMpLayer* pLayer, const FString& NodeName)
	{
		if (pLayer == nullptr)
		{
			return nullptr;
		}
		// ���ڵ���ӵ�ͼ����
		int nodeId = SceneManager->GetSceneFactory()->CreateNodeUID();
		IMpNode* pNode = SceneManager->GetSceneFactory()->CreateNode(pGraphics, nodeId);
		SceneManager->AddNode(pLayer, pNode);
		return pNode;
	}
};
