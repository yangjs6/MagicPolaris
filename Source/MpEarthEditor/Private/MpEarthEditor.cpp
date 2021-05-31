// Copyright Epic Games, Inc. All Rights Reserved.

#include "MpEarthEditor.h"

#define LOCTEXT_NAMESPACE "FMpEarthEditorModule"

void FMpEarthEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FMpEarthEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMpEarthEditorModule, MpEarthEditor)