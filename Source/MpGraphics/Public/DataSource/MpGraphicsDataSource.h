#pragma once

#include "CoreMinimal.h"
#include "Interfaces/IMpDataSource.h"
#include "MpGraphicsViewModel.h"
#include "MpGraphicsDataSource.generated.h"

class UMpGraphics;
class UMpGraphicsViewModel;

UCLASS(Abstract)
class MPGRAPHICS_API UMpGraphicsDataSource : 
	public UObject,
	public IMpDataSource
{
	GENERATED_BODY()
public:
	UMpGraphicsDataSource();

	UFUNCTION()
	virtual bool Load(TScriptInterface<IMpGraphicsDataReceiver> Receiver) { return false; }

	DECLARE_DELEGATE_OneParam(FOnDataSourceChanged, UMpGraphicsDataSource*);
	FOnDataSourceChanged OnDataSourceChangedEvent;

	void MarkDataSourceChanged();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

};

UCLASS(Abstract)
class MPGRAPHICS_API AMpGraphicsDataSourceActor :
	public AMpGraphicsViewModelActor,
	public IMpGraphicsDataReceiver
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, Category = "DataSource")
	UMpGraphicsDataSource* DataSource;

#if WITH_EDITOR
	virtual void PreEditChange(FProperty* PropertyThatWillChange) override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

protected:

	virtual void PostLoad() override;

	virtual void ReceiveGraphics(UMpGraphics* pGraphics);

	virtual void ReloadData();

	void OnDataSourceChangedCallback(UMpGraphicsDataSource* InDataSource);
};
