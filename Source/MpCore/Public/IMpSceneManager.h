#pragma once

#include "IMpUnknown.h"
#include "IMpObjectContainer.h"

class IMpGraphics;
class IMpNode;
class IMpLayer;
class IMpScene;
class IMpSceneFactory;
class IMpSceneManager;

//! 场景物体基类
class IMpSceneObject : public IMpUnknown
{
protected:
	int ObjectID;
	FString ObjectName;
	FString ObjectNameID;
	//! 可见性
	bool bVisiable;
	//! 可检测性
	bool bDetectable;
	//! 可编译性
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

	//! 获得id
	virtual int GetID() { return ObjectID; }

	//! 获得名称
	virtual const FString& GetName() { return ObjectName; }

	//! 设置名称
	virtual void SetName(const FString& name) { ObjectName = name; }

	//! 获得名称ID
	virtual const FString& GetNameID() { return ObjectNameID; }

	//! 设置名称ID
	virtual void SetNameID(const FString& name) { ObjectNameID = name; }

	//! 设置可见性
	virtual void SetVisiable(bool visiable) { bVisiable = visiable; }

	//! 设置编辑性
	virtual void SetEditable(bool editable) { bEditable = editable; }

	//! 设置可检测性
	virtual void SetDetectable(bool detectable) { bDetectable = detectable; }

	//! 获得可见性
	virtual bool GetVisiable() { return bVisiable; }

	//! 获得编辑性
	virtual bool GetEditable() { return bEditable; }

	//! 获得可检测性
	virtual bool GetDetectable() { return bDetectable; }

	////////////////////////////////////////////////////////////////////////////
	// 序列化到数据流
	virtual int Serialize(FString& data) { return 0; }

	// 反序列化到结构体
	virtual int Deserialize(const FString& data) { return 0; }
};

//! 节点事件监听
class IMpNodeEventListener : public IMpUnknown
{
public:
	//! 当属性发生变化需要更新时
	virtual void OnAttrChanged(IMpNode* pNode, int reason = 0) = 0;

	//! 当图形发生变化需要更新时
	virtual void OnGraphicsUpdated(IMpNode* pNode, int reason = 0) = 0;
};

//!图形，基本的操作对象
class IMpNode : public IMpSceneObject
{
public:
	//! 挂载图层
	virtual void AttachLayer(IMpLayer* pLayer) = 0;

	//! 获得图层
	virtual IMpLayer* GetLayer() = 0;

	//////////////////////////////////////////////////////////////////////////
	//! 获得图形
	virtual IMpGraphics* GetGraphics() = 0;

	//////////////////////////////////////////////////////////////////////////
	//! 注册事件监听
	virtual void RegisterEventListener(IMpNodeEventListener* pEventListener) = 0;

	//! 解除事件监听
	virtual void UnRegisterEventListener(IMpNodeEventListener* pEventListener) = 0;

	//! 当属性发生变化需要更新时
	virtual void OnAttrChanged(int reason = 0) = 0;

	//! 当图形发生变化需要更新时
	virtual void OnGraphicsUpdated(int reason = 0) = 0;
	////////////////////////////////////////////////////////////////////////////
	////! 准备内容
	//virtual void PrepareViewContent(IMpViewDrawContext* pView) = 0;
};

//!标绘场景图层
class IMpLayer : public IMpSceneObject,
	public IMpObjectContainer<IMpNode>
{
public:
	//! 挂载图层
	virtual void AttachScene(IMpScene* pScene) = 0;

	//! 获得归属的场景
	virtual IMpScene* GetScene() = 0;

	//! 根据名称获得节点
	virtual IMpNode* GetNodeByName(const FString& nodeName) = 0;
};

//!标绘场景、方案
class IMpScene : public IMpSceneObject,
	public IMpObjectContainer<IMpLayer>
{
public:
	//! 根据名称获得图层
	virtual IMpLayer* GetLayerByName(const FString& LayerName) = 0;

	//! 根据名称获得节点
	virtual IMpNode* GetNodeByName(const FString& nodeName) = 0;

	//! 设置保存文件名
	virtual void SetSaveFilePath(const FString& filePath) = 0;

	//! 获得保存文件名
	virtual const FString& GetSaveFilePath() = 0;
};

//!标绘场景交互事件监听
class IMpSceneEventListener : public IMpUnknown
{
public:
	//! 创建场景事件
	virtual void OnCreateScene(int sceneId) = 0;

	//! 删除场景事件
	virtual void OnDeleteScene(int sceneId) = 0;

	//! 选中场景事件
	virtual void OnSelectScene(int sceneId) = 0;

	//! 清空场景事件
	virtual void OnClearScene(int sceneId) = 0;

	//! 创建图层事件
	virtual void OnAddLayer(int sceneId, int layerId) = 0;

	//! 删除图层事件
	virtual void OnDeleteLayer(int layerId) = 0;

	//! 设置图层可见性事件
	virtual void OnSetLayerVisiable(int id, bool visible) = 0;

	//! 设置图层可编辑事件
	virtual void OnSetLayerEditable(int id, bool editable) = 0;

	//! 选中图层事件
	virtual void OnSelectLayer(int layerId) = 0;

	//! 清空图层事件
	virtual void OnClearLayer(int layerId) = 0;

	//! 创建图形事件
	virtual void OnAddNode(int layerId, int id) = 0;

	//! 删除图形事件
	virtual void OnDeleteNode(int id) = 0;

	//! 设置图形可见性事件
	virtual void OnSetNodeVisible(int id, bool visible) = 0;

	//! 设置图形可编辑事件
	virtual void OnSetNodeEditable(int id, bool editable) = 0;

	//! 选中图形事件
	virtual void OnSelectNodeByID(int id) = 0;

	//! 清空选中事件
	virtual void OnSelectNothing() = 0;

	//! 场景更新 0 为名字修改 1-4为节点上下移动标志
	virtual void OnSceneUpdated(int id, int reason) = 0;

	//! 图层更新 0 为名字修改 1-4为节点上下移动标志
	virtual void OnLayerUpdated(int id, int reason) = 0;

	//! 图形更新 0 为名字修改 1-4为节点上下移动标志
	virtual void OnNodeUpdated(int id, int reason) = 0;

	//! 当节点属性发生变化需要更新时
	virtual void OnNodeAttrChanged(int id, int reason = 0) = 0;

	//! 当节点图形发生变化需要更新时
	virtual void OnNodeGraphicsUpdated(int id, int reason = 0) = 0;
};

//!标绘场景管理器
class IMpSceneManager : public IMpUnknown,
	public IMpObjectContainer<IMpScene>
{
public:
	//! 获得场景工厂
	virtual IMpSceneFactory* GetSceneFactory() = 0;

	//! 注册事件监听
	virtual void RegisterEventListener(IMpSceneEventListener* pEventListener) = 0;

	//! 解除事件监听
	virtual void UnRegisterEventListener(IMpSceneEventListener* pEventListener) = 0;

	//! 导入场景文件
	virtual IMpScene* ImportScenefile(const FString& fileName) = 0;

	//! 导出场景文件
	virtual bool ExportScenefile(IMpScene* pScene, const FString& fileName) = 0;

	//////////////////////////////////////////////////////////////////////////

	//! 创建场景
	virtual IMpScene* CreateScene(int sceneId, const FString& name = "") = 0;

	//! 删除场景
	virtual void DeleteScene(IMpScene* pScene) = 0;

	//! 设置场景名称
	virtual void SetSceneName(IMpScene* pScene, const FString& name) = 0;

	//! 选中场景
	virtual void SelectScene(IMpScene* pScene) = 0;

	//! 清空场景
	virtual void ClearScene(IMpScene* pScene) = 0;

	//! 根据ID获得场景
	virtual IMpScene* GetSceneByID(int sceneId) = 0;

	//! 根据名称获得场景
	virtual IMpScene* GetSceneByName(const FString& sceneName) = 0;

	//! 根据场景名字得到ID
	virtual int GetSceneIDByName(const FString& mame) = 0;

	//! 根据场景ID得到名字
	virtual const FString& GetSceneNameByID(int id) = 0;

	//////////////////////////////////////////////////////////////////////////

	//! 创建图层
	virtual IMpLayer* CreateLayer(IMpScene* pScene, int layerId, const FString& name = "") = 0;

	//! 删除图层
	virtual void DeleteLayer(IMpLayer* pLayer) = 0;

	//! 设置图层名称
	virtual void SetLayerName(IMpLayer* pLayer, const FString& name) = 0;

	//! 设置图层可见
	virtual void SetLayerVisiable(IMpLayer* pLayer, bool visible) = 0;

	//! 设置图层可编辑
	virtual void SetLayerEditable(IMpLayer* pLayer, bool editable) = 0;

	//! 选中图层
	virtual void SelectLayer(IMpLayer* pLayer) = 0;

	//! 清空图层
	virtual void ClearLayer(IMpLayer* pLayer) = 0;

	//! 根据ID获得图层，如果sceneId为0从所有场景中查找，否则只查找单个场景
	virtual IMpLayer* GetLayerByID(int layerId, int sceneId = 0) = 0;

	//! 根据名称获得图层
	virtual IMpLayer* GetLayerByName(const FString& layerName) = 0;

	//! 根据图层名字得到ID
	virtual int GetLayerIDByName(const FString& mame) = 0;

	//! 根据图层ID得到名字
	virtual const FString& GetLayerNameByID(int id) = 0;

	//! 移动图层 其中nStep取值在[-N,N]，N为图层数，返回实际移动的步数
	virtual int MoveLayer(IMpLayer* pLayer, int nStep) = 0;

	//! 上移图层
	virtual void MoveLayerUp(IMpLayer* pLayer) = 0;

	//! 下移图层
	virtual void MoveLayerDown(IMpLayer* pLayer) = 0;

	//! 置顶图层
	virtual void MoveLayerTop(IMpLayer* pLayer) = 0;

	//! 置底图层
	virtual void MoveLayerBottom(IMpLayer* pLayer) = 0;

	//////////////////////////////////////////////////////////////////////////

	//! 添加图形
	virtual void AddNode(IMpLayer* pLayer, IMpNode* pNode) = 0;

	//! 删除图形
	virtual void DeleteNode(IMpNode* pNode) = 0;

	//! 删除所有选中节点
	virtual void DeleteSelectedNodes() = 0;

	////! 复制图形
	//virtual IMpNode* copyNode(IMpNode* pNode)=0;

	//! 复制图形，从节点到文本
	virtual FString CopyNode(IMpNode* pNode) = 0;

	//! 粘贴图形，从文本到节点
	virtual IMpNode* ParseNode(const FString& pNodeString) = 0;

	//! 设置图形名称
	virtual void SetNodeName(IMpNode* pNode, const FString& name) = 0;

	//! 设置图形可见
	virtual void SetNodeVisible(IMpNode* pNode, bool visible) = 0;

	//! 设置图形可编辑
	virtual void SetNodeEditable(IMpNode* pNode, bool editable) = 0;

	//! 选中图形
	virtual void SelectNode(IMpNode* pNode) = 0;

	//! 根据ID获得节点，如果sceneId为0从所有场景中查找，否则只查找单个场景
	virtual IMpNode* GetNodeByID(int nodeID, int layerId = 0, int sceneId = 0) = 0;

	//! 根据名称获得节点
	virtual IMpNode* GetNodeByName(const FString& nodeName) = 0;

	//! 根据节点名字得到ID
	virtual int GetNodeIDByName(const FString& mame) = 0;

	//! 根据节点ID得到名字
	virtual const FString& GetNodeNameByID(int id) = 0;

	//! 移动图层 其中nStep取值在[-N,N]，N为图层数，返回实际移动的步数
	virtual int MoveNode(IMpNode* pNode, int nStep) = 0;

	//! 上移节点
	virtual void MoveNodeUp(IMpNode* pNode) = 0;

	//! 下移节点
	virtual void MoveNodeDown(IMpNode* pNode) = 0;

	//! 置顶节点
	virtual void MoveNodeTop(IMpNode* pNode) = 0;

	//! 置底节点
	virtual void MoveNodeBottom(IMpNode* pNode) = 0;

	//////////////////////////////////////////////////////////////////////////

	//! 取消选中
	virtual void SelectNothing() = 0;

	//! 获得当前激活的场景ID
	virtual IMpScene* GetActiveScene() = 0;

	//! 获得当前激活的图层ID
	virtual IMpLayer* GetActiveLayer() = 0;

	//! 获得当前激活的图形ID
	virtual IMpNode* GetActiveNode() = 0;

	//////////////////////////////////////////////////////////////////////////
	//! 获得多选节点数量
	virtual int GetSelectedNodeCount() = 0;

	//! 获得第一个选中节点，如果无选中对象，返回NULL
	virtual IMpNode* GetFirstSelectedNode() = 0;

	//! 获得下一个选中节点，如果当前为最后一个对象，返回NULL
	virtual IMpNode* GetNextSelectedNode() = 0;

	//! 获得上一个选中节点，如果当前为第一个对象，返回NULL
	virtual IMpNode* GetPrevSelectedNode() = 0;

	//! 选中节点扩展接口，为了实现多选节点
	virtual bool SelectNodeEx(IMpNode* pNode, bool bSelect = true) = 0;
};

//! 图形工厂
class IMpSceneFactory : public IMpUnknown
{
public:
	//! 设置uid的掩码，用于不同机器创建出来的uid之间确保惟一性
	virtual void ResetUIDMask(int gid = 0, int mid = 0, int nid = 0) = 0;

	//! 创建场景的惟一id
	virtual int CreateSceneUID() = 0;

	//! 创建图层的惟一id
	virtual int CreateLayerUID() = 0;

	//! 创建图形的惟一id
	virtual int CreateNodeUID() = 0;

	//! 创建场景
	virtual IMpScene* CreateScene(int id, const FString& name = "") = 0;

	//! 创建图层
	virtual IMpLayer* CreateLayer(int id, const FString& name = "") = 0;

	//! 创建节点
	virtual IMpNode* CreateNode(IMpGraphics* pGraphics, int id, const FString& name = "") = 0;

};

class FMpSceneUtils
{
public:
	//获得当前场景，当场景为空时创建一个新的场景
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

	//获得当前图层； 当图层为空时创建一个新的图层
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
		// 将节点添加到图层中
		int nodeId = SceneManager->GetSceneFactory()->CreateNodeUID();
		IMpNode* pNode = SceneManager->GetSceneFactory()->CreateNode(pGraphics, nodeId);
		SceneManager->AddNode(pLayer, pNode);
		return pNode;
	}
};
