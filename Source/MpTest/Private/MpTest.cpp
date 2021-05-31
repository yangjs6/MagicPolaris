// Copyright Epic Games, Inc. All Rights Reserved.

#include "MpTest.h"

#include "MpGraphicsViewModel.h"

#include "MpGraphicsFactory.h"
#include "Graphics/MpBoxGraphics.h"


#define LOCTEXT_NAMESPACE "FMpTestModule"

void Test1()
{
 	//UMpView* View = FMpCoreModule::CreateView("Test1");
	////UMpDataSource* DataSource = FMpDataSourceModule::CreateDataSource("Type","Filename","DataSourceName");
	////Viewer->AddDataSource(DataSource);

	UMpBoxGraphics* BoxGraphics = FMpGraphicsFactory::CreateGraphics<UMpBoxGraphics>();
	BoxGraphics->Dimensions = FVector(90000.0, 90000.0, 90000.0);
	//UMpEntity* Entity1 = FMpGraphicsEntityFactory::CreateShapeEntity(BoxGraphics);

	FGeoPosition Position = FGeoPosition::FromDegrees(-108.0, 45.0, 100);
	BoxGraphics->GetVertexSource()->PushPoint(Position);

	FMpGraphicsViewModel* ViewModel = FMpGraphicsViewModel::Get();
	ViewModel->AddGraphics(BoxGraphics);

}


void FMpTestModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	Test1();
}

void FMpTestModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMpTestModule, MpTest)