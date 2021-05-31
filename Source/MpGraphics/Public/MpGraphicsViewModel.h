#pragma once

#include "IMpViewModel.h"

class UMpGraphicsBase;

class IMpGraphics;
class IMpSchemaManager;
class IMpSceneManager;
class IMpGraphicsFactory;
class IMpVertexSourceFactory;
class IMpGraphicsDrawContext;

class MPGRAPHICS_API FMpGraphicsViewModel : public IMpViewModel, FGCObject
{
public:
	static FMpGraphicsViewModel* Get();
	static FString DefaultSceneName;
	static FString DefaultLayerName;
	static FString DefaultNodeName;

	FMpGraphicsViewModel();
	~FMpGraphicsViewModel();

	virtual void AddReferencedObjects(FReferenceCollector& Collector);


	IMpSceneManager* GetSceneManager() { return SceneManager.Get(); }

	void AddGraphics(UMpGraphicsBase* Graphics, const FString& NodeName = DefaultNodeName, const FString& LayerName = DefaultLayerName, const FString& SceneName = DefaultSceneName);

public:
	//!view更新 (同步)
	virtual void OnUpdateSyn(IMpSceneView* SceneView, float DeltaTime);
	//!view更新 (异步)
	virtual void OnUpdateAsyn(IMpSceneView* SceneView, float DeltaTime);
	//!view回收 (同步)
	virtual void OnRetrieveSyn(IMpSceneView* SceneView, float DeltaTime);
	//!view回收 (异步)
	virtual void OnRetrieveAsyn(IMpSceneView* SceneView, float DeltaTime);

	//!准备绘制，返回false表示不支持该视图绘制
	virtual bool OnPreDraw(IMpSceneView* SceneView);
	//!结束绘制
	virtual void OnPostDraw(IMpSceneView* SceneView);

protected:
	IMpSchemaManager* SchemaManager;
	TSharedPtr<IMpSceneManager> SceneManager;
	IMpGraphicsDrawContext* DrawContext;

	IMpGraphicsFactory* GraphicsFactory;
	IMpVertexSourceFactory* VertexSourceFactory;

	TArray<UMpGraphicsBase*> GraphicsToUpdate;

	TArray<UMpGraphicsBase*> AddedGraphics;
};
