// Copyright Epic Games, Inc. All Rights Reserved.

#include "DataSource/MpGraphicsDataSource.h"
#include "MpGraphicsViewModel.h"

UMpGraphicsDataSource::UMpGraphicsDataSource()
{

}

void UMpGraphicsDataSource::MarkDataSourceChanged()
{
	OnDataSourceChangedEvent.ExecuteIfBound(this);
}

#if WITH_EDITOR
void UMpGraphicsDataSource::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	MarkDataSourceChanged();
}
#endif

#if WITH_EDITOR

void AMpGraphicsDataSourceActor::PreEditChange(FProperty* PropertyThatWillChange)
{
	Super::PreEditChange(PropertyThatWillChange);

	const FName PropertyName = PropertyThatWillChange->GetFName();
	
	if (PropertyName == GET_MEMBER_NAME_CHECKED(AMpGraphicsDataSourceActor, DataSource))
	{
		if (DataSource)
		{
			DataSource->OnDataSourceChangedEvent.Unbind();
		}
	}
}

void AMpGraphicsDataSourceActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName PropertyName = (PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None);

	// Reconstruct node to show updates to PinFriendlyNames.
	if (PropertyName == GET_MEMBER_NAME_CHECKED(AMpGraphicsDataSourceActor, DataSource))
	{
		ReloadData();

		if (DataSource)
		{
			DataSource->OnDataSourceChangedEvent.BindUObject(this, &AMpGraphicsDataSourceActor::OnDataSourceChangedCallback);
		}
	}
}
#endif

void AMpGraphicsDataSourceActor::PostLoad()
{
	Super::PostLoad();

	ReloadData();

	if (DataSource)
	{
		DataSource->OnDataSourceChangedEvent.BindUObject(this, &AMpGraphicsDataSourceActor::OnDataSourceChangedCallback);
	}
}

void AMpGraphicsDataSourceActor::ReceiveGraphics(UMpGraphics* pGraphics)
{
	ViewModel->AddGraphics(pGraphics);
}

void AMpGraphicsDataSourceActor::ReloadData()
{
	ViewModel->ClearGraphics();
	if (DataSource)
	{
		DataSource->Load(this);
	}
}

void AMpGraphicsDataSourceActor::OnDataSourceChangedCallback(UMpGraphicsDataSource* InDataSource)
{
	if (InDataSource == DataSource)
	{
		ReloadData();
	}
}
