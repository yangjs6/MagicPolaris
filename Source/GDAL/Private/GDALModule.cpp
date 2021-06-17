// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "GDALModule.h"

#include "Interfaces/IPluginManager.h"
#include "HAL/PlatformProcess.h"

#include "gdal_priv.h"
#include "gdalwarper.h"
#include "ogr_api.h"
#include "ogrsf_frmts.h"
#include "Misc/Paths.h"

class GIS_Transformer_Wrapper
{
private:
	double Origin_Lng;
	double Origin_Lat;
	
	OGRCoordinateTransformation* GOGRCT_LLtoUTM;
	OGRCoordinateTransformation* GOGRCT_UTMtoLL;
	bool CheckNewOrigin(const double lonOrigin, const double latOrigin);
	void CreateNewTransformer(const double lonOrigin, const double latOrigin);
	void CreateNewTransformer_AES(const double lonOrigin, const double latOrigin);
	

public:
	GIS_Transformer_Wrapper();
	~GIS_Transformer_Wrapper();
	void Release();
	void LLtoUTM(const double lonOrigin, const double latOrigin, double& x, double& y);
	void UTMtoLL(const double lonOrigin, const double latOrigin, double& x, double& y);

	void LLtoUTM_AES(const double lonOrigin, const double latOrigin, double& x, double& y);
	void UTMtoLL_AES(const double lonOrigin, const double latOrigin, double& x, double& y);

	
};




GIS_Transformer_Wrapper::GIS_Transformer_Wrapper()
	: Origin_Lng(116.43859863)
	, Origin_Lat(39.95080618)
	, GOGRCT_LLtoUTM(nullptr)
	, GOGRCT_UTMtoLL(nullptr)
{

}
GIS_Transformer_Wrapper::~GIS_Transformer_Wrapper()
{
	Release();
}

bool GIS_Transformer_Wrapper::CheckNewOrigin(const double lonOrigin, const double latOrigin)
{
	double deltaLng = lonOrigin - Origin_Lng;
	double deltaLat = latOrigin - Origin_Lat;
	if (FMath::IsNearlyZero(deltaLng) && FMath::IsNearlyZero(deltaLat))
		return false;

	return true;
}

void GIS_Transformer_Wrapper::CreateNewTransformer(const double lonOrigin, const double latOrigin)
{
	Release();

	//LLtoUTM
	{
		OGRSpatialReference oSourceSRS, oTargetSRS;
		oSourceSRS.importFromProj4("+proj=longlat +ellps=WGS84 +datum=WGS84 +no_defs");
		oTargetSRS.importFromProj4(
			("+proj=ortho +lon_0=" + std::to_string(lonOrigin) +
				" +lat_0=" + std::to_string(latOrigin) +
				" +a=6371010 +units=m +no_defs").c_str());

		GOGRCT_LLtoUTM = OGRCreateCoordinateTransformation(&oSourceSRS, &oTargetSRS);
	}

	//UTMtoLL
	{
		OGRSpatialReference oSourceSRS, oTargetSRS;
		oSourceSRS.importFromProj4("+proj=longlat +ellps=WGS84 +datum=WGS84 +no_defs");
		oTargetSRS.importFromProj4(
			("+proj=ortho +lon_0=" + std::to_string(lonOrigin) +
				" +lat_0=" + std::to_string(latOrigin) +
				" +a=6371010 +units=m +no_defs").c_str());

		GOGRCT_UTMtoLL = OGRCreateCoordinateTransformation(&oTargetSRS, &oSourceSRS);
	}

}


void GIS_Transformer_Wrapper::CreateNewTransformer_AES(const double lonOrigin, const double latOrigin)
{
	Release();

	//LLtoUTM
	{
		OGRSpatialReference oSourceSRS, oTargetSRS;
		oSourceSRS.importFromProj4("+proj=longlat +ellps=WGS84 +datum=WGS84 +no_defs");
		oTargetSRS.importFromProj4(
			("+proj=ortho +lon_0=" + std::to_string(lonOrigin) +
				" +lat_0=" + std::to_string(latOrigin) +
				" +ellps=WGS84 +datum=WGS84 +units=m +no_defs").c_str());

		GOGRCT_LLtoUTM = OGRCreateCoordinateTransformation(&oSourceSRS, &oTargetSRS);
	}

	//UTMtoLL
	{
		OGRSpatialReference oSourceSRS, oTargetSRS;
		oSourceSRS.importFromProj4("+proj=longlat +ellps=WGS84 +datum=WGS84 +no_defs");
		oTargetSRS.importFromProj4(
			("+proj=ortho +lon_0=" + std::to_string(lonOrigin) +
				" +lat_0=" + std::to_string(latOrigin) +
				" +ellps=WGS84 +datum=WGS84 +units=m +no_defs").c_str());

		GOGRCT_UTMtoLL = OGRCreateCoordinateTransformation(&oTargetSRS, &oSourceSRS);
	}

}


void GIS_Transformer_Wrapper::Release()
{
	if (GOGRCT_LLtoUTM != nullptr)
		OGRCoordinateTransformation::DestroyCT(GOGRCT_LLtoUTM);

	if (GOGRCT_UTMtoLL != nullptr)
		OGRCoordinateTransformation::DestroyCT(GOGRCT_UTMtoLL);

	GOGRCT_LLtoUTM = nullptr;
	GOGRCT_UTMtoLL = nullptr;
}

void GIS_Transformer_Wrapper::LLtoUTM(const double lonOrigin, const double latOrigin, double& x, double& y)
{
	bool bNeedCreated = CheckNewOrigin(lonOrigin, latOrigin);
	if (GOGRCT_LLtoUTM == nullptr || GOGRCT_UTMtoLL == nullptr)
		bNeedCreated = true;

	if (bNeedCreated)
	{
		Origin_Lng = lonOrigin;
		Origin_Lat = latOrigin;
		CreateNewTransformer(Origin_Lng, Origin_Lat);
	}

	
	if (GOGRCT_LLtoUTM != nullptr)
	{
		GOGRCT_LLtoUTM->Transform(1, &x, &y);
	}
}

void GIS_Transformer_Wrapper::UTMtoLL(const double lonOrigin, const double latOrigin, double& x, double& y)
{
	bool bNeedCreated = CheckNewOrigin(lonOrigin, latOrigin);
	if (GOGRCT_LLtoUTM == nullptr || GOGRCT_UTMtoLL == nullptr)
		bNeedCreated = true;

	if (bNeedCreated)
	{
		Origin_Lng = lonOrigin;
		Origin_Lat = latOrigin;
		CreateNewTransformer(Origin_Lng, Origin_Lat);
	}

	if (GOGRCT_UTMtoLL)
	{
		GOGRCT_UTMtoLL->Transform(1, &x, &y);
	}
}


void GIS_Transformer_Wrapper::LLtoUTM_AES(const double lonOrigin, const double latOrigin, double& x, double& y)
{
	{
		bool bNeedCreated = CheckNewOrigin(lonOrigin, latOrigin);
		if (GOGRCT_LLtoUTM == nullptr || GOGRCT_UTMtoLL == nullptr)
			bNeedCreated = true;

		if (bNeedCreated)
		{
			Origin_Lng = lonOrigin;
			Origin_Lat = latOrigin;
			CreateNewTransformer_AES(Origin_Lng, Origin_Lat);
		}


		if (GOGRCT_LLtoUTM != nullptr)
		{
			GOGRCT_LLtoUTM->Transform(1, &x, &y);
		}
	}
}
void GIS_Transformer_Wrapper::UTMtoLL_AES(const double lonOrigin, const double latOrigin, double& x, double& y)
{
	bool bNeedCreated = CheckNewOrigin(lonOrigin, latOrigin);
	if (GOGRCT_LLtoUTM == nullptr || GOGRCT_UTMtoLL == nullptr)
		bNeedCreated = true;

	if (bNeedCreated)
	{
		Origin_Lng = lonOrigin;
		Origin_Lat = latOrigin;
		CreateNewTransformer_AES(Origin_Lng, Origin_Lat);
	}

	if (GOGRCT_UTMtoLL)
	{
		GOGRCT_UTMtoLL->Transform(1, &x, &y);
	}
}

static GIS_Transformer_Wrapper G_Gis_Transformer;


#if PLATFORM_LINUX

/* Boilerplate
 *****************************************************************************/

#include "Misc/MonolithicHeaderBoilerplate.h"
MONOLITHIC_HEADER_BOILERPLATE()


#endif 

static void* LibraryHandle;

#define LOCTEXT_NAMESPACE "FGDALModule"

void FGDALModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

		// Get the base directory of this plugin
	FString BaseDir = IPluginManager::Get().FindPlugin("MagicPolaris")->GetBaseDir();
	// Add on the relative location of the third party dll and load it
	FString LibraryPath;

#if PLATFORM_WINDOWS
	
	FString DllPath = FPaths::Combine(*BaseDir, TEXT("Source/GDAL/dll/Win64"));
	FPlatformProcess::AddDllDirectory(*DllPath);
	LibraryPath = FPaths::Combine(*DllPath, TEXT("gdal300.dll"));


#elif PLATFORM_LINUX
	FString LibPath = FPaths::Combine(*BaseDir, TEXT("Source/GDAL/Linux/lib"));
	LibraryPath = FPaths::Combine(*LibPath, TEXT("libgdal.so"));
#endif 

	LibraryHandle = !LibraryPath.IsEmpty() ? FPlatformProcess::GetDllHandle(*LibraryPath) : nullptr;
	if (!LibraryHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load third party library: gdal300.dll "));
		return;
	}


	UE_LOG(LogTemp, Log, TEXT("============= GDAL  OnStartup  ============="));
	UE_LOG(LogTemp, Log, TEXT("==== GDALAllRegister ===="));

	GDALAllRegister();
	GDALDriverManager* pDriverManager = GetGDALDriverManager();
	for (size_t i = 0; i < pDriverManager->GetDriverCount(); i++)
	{
		const char* pDriverDesc = pDriverManager->GetDriver(i)->GetDescription();
		UE_LOG(LogTemp, Log, TEXT("GDALDriver[%d]:%s"), i, ANSI_TO_TCHAR(pDriverDesc));
	}

	CPLSetConfigOption("GDAL_CACHEMAX", "1000");
	CPLSetConfigOption("OGR_PG_CURSOR_PAGE", "10000");
	CPLSetConfigOption("SHAPE_ENCODING", "");

	UE_LOG(LogTemp, Log, TEXT("==== GDALAllRegister ===="));

}

void FGDALModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UE_LOG(LogTemp, Log, TEXT("============= GDAL OnShutdown ============="));
	G_Gis_Transformer.Release();

	FPlatformProcess::FreeDllHandle(LibraryHandle);
	LibraryHandle = nullptr;


}


void FGDALModule::LLtoUTM(const double lonOrigin, const double latOrigin, double& x, double& y)
{
	G_Gis_Transformer.LLtoUTM(lonOrigin, latOrigin, x, y);
}
void FGDALModule::UTMtoLL(const double lonOrigin, const double latOrigin, double& x, double& y)
{
	G_Gis_Transformer.UTMtoLL(lonOrigin, latOrigin, x, y);
}



void FGDALModule::LLtoUTM_AES(const double lonOrigin, const double latOrigin, double& x, double& y)
{
	G_Gis_Transformer.LLtoUTM_AES(lonOrigin, latOrigin, x, y);
}
void FGDALModule::UTMtoLL_AES(const double lonOrigin, const double latOrigin, double& x, double& y)
{
	G_Gis_Transformer.UTMtoLL_AES(lonOrigin, latOrigin, x, y);
}


#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FGDALModule, GDAL)