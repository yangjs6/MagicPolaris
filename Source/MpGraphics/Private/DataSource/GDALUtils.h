// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GeometryUtils.h"

#include "gdal_priv.h"
#include "gdalwarper.h"
#include "ogr_api.h"
#include "ogrsf_frmts.h"

class FGDALUtils
{
public:
	static GDALDataset* OpenDataset(OGRLayer** Layer, const char* fileName, const char* layerName);

	static void CloseDataset(GDALDataset* Dataset);

	static TArray<FName> GetFieldNames(OGRLayer* poLayer);

	static FString ToString(const char* str);

	static EMpFieldType ToFieldType(OGRFieldType Type, OGRFieldSubType SubType);

	static FVariant ToFieldValue(OGRFeature* Feature, int i, EMpFieldType Type);

	static EMpGeometryType ToGeomType(OGRwkbGeometryType Type);

	static TArray< TArray<FGeoPosition>> ToGeomPoints(OGRGeometry* Geometry);

	static OGRGeometryCollection* ToGeometryCollection(OGRGeometry* Geometry);
};
