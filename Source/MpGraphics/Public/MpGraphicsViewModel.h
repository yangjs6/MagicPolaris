#pragma once

#include "MpViewModelActor.h"
#include "Interfaces/IMpGraphics.h"
#include "Interfaces/IMpViewModel.h"
#include "MpGraphicsViewModel.generated.h"

class UMpGraphics;


UCLASS()
class MPGRAPHICS_API UMpGraphicsViewModel : 
	public UObject,
	public IMpViewModel
{
	GENERATED_BODY()
public:

	UMpGraphicsViewModel();
	~UMpGraphicsViewModel();

	UPROPERTY(Category = "ViewModel", EditAnywhere)
		bool bEnable = true;

	UPROPERTY(Category = "ViewModel", EditAnywhere)
		bool bUseUpdateAsyn = false;

	//! 返回是否启用
	virtual bool IsEnable(IMpSceneView* SceneView) { return bEnable; }

	void InitDrawContext(IMpGraphicsDrawContext* InDrawContext) { DrawContext = InDrawContext; }

	void ClearGraphics();

	void AddGraphics(UMpGraphics* Graphics);

public:
	//!view更新 (同步)
	virtual void OnUpdateSyn(IMpSceneView* SceneView, float DeltaTime);
	//!view更新 (异步)
	virtual void OnUpdateAsyn(IMpSceneView* SceneView, float DeltaTime);
	//!view回收 (同步)
	virtual void OnRetrieveSyn(IMpSceneView* SceneView, float DeltaTime);
	//!view回收 (异步)
	virtual void OnRetrieveAsyn(IMpSceneView* SceneView, float DeltaTime);

protected:

	IMpGraphicsDrawContext* DrawContext;

	UPROPERTY()
	TArray<UMpGraphics*> AllGraphics;

	TArray<UMpGraphics*> GraphicsToUpdate;
	TArray<UMpGraphics*> GraphicsToRetrieve;
};


UCLASS()
class MPGRAPHICS_API UMpGraphicsStyleBase :
	public UObject
	, public IMpGraphicsStyle
{
	GENERATED_BODY()
public:
	virtual UMaterialInterface* GetMaterialByName(const FName& InMaterial) { return nullptr; }

	virtual UMaterialInterface* GetMaterialById(const int& InMaterialId) { return MaterialById.FindRef(InMaterialId); }

	UPROPERTY(Category = "Graphics | Material", EditAnywhere)
	TMap<int, UMaterialInterface*> MaterialById;
};

UCLASS(Abstract)
class MPGRAPHICS_API AMpGraphicsViewModelActor :
	public AMpViewModelActor,
	public IMpGraphicsDrawContext
{
	GENERATED_BODY()
public:
	AMpGraphicsViewModelActor();

	virtual IMpViewModel* GetViewModel() { return ViewModel; }

	virtual bool RegisterDrawComponent(USceneComponent* Component);
	virtual bool UnRegisterDrawComponent(USceneComponent* Component);


	virtual UMaterialInterface* GetDefaultMaterial() { return DefaultMaterial ? DefaultMaterial : UMaterial::GetDefaultMaterial(MD_Surface); }

	virtual IMpGraphicsStyle* GetStyle(const FName& InName) { return GraphicsStyleMap.FindRef(InName); }


	UPROPERTY(Category = "Graphics | Material", EditAnywhere)
		UMaterialInterface* DefaultMaterial;

	UPROPERTY(Category = "Graphics | Material", EditAnywhere)
		TMap<FName, UMpGraphicsStyleBase*> GraphicsStyleMap;

protected:

	UPROPERTY(Category = "ViewModel", EditAnywhere)
		UMpGraphicsViewModel* ViewModel;
};

