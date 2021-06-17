// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

class GDAL_API FGDALModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	static void LLtoUTM(const double lonOrigin, const double latOrigin, double& x, double& y);
	static void UTMtoLL(const double lonOrigin, const double latOrigin, double& x, double& y);
	
	static void LLtoUTM_AES(const double lonOrigin, const double latOrigin, double& x, double& y);
	static void UTMtoLL_AES(const double lonOrigin, const double latOrigin, double& x, double& y);

};
