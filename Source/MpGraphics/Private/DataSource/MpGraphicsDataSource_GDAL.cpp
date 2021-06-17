// Copyright Epic Games, Inc. All Rights Reserved.

#include "DataSource/MpGraphicsDataSource_GDAL.h"
#include "GDALUtils.h"


void LoadFeature(OGRFeature* Feature, const TArray<FName>& FieldNames, TScriptInterface<IMpGraphicsDataReceiver> Receiver)
{
	if (!Feature) return;

	OGRGeometry* Geometry = Feature->GetGeometryRef();
	if (!Geometry) return;


	// 1 get field values
	OGRFeatureDefn* poFDefn = Feature->GetDefnRef();
	int numField = poFDefn->GetFieldCount();
	TArray<FVariant> FieldValues;
	FieldValues.SetNum(numField);
	for (int i = 0; i < numField; i++)
	{
		OGRFieldDefn* poFieldDefn = poFDefn->GetFieldDefn(i);
		poFieldDefn->GetNameRef();

		int Precision = poFieldDefn->GetPrecision();
		OGRFieldType FieldType = poFieldDefn->GetType();
		OGRFieldSubType SubType = poFieldDefn->GetSubType();

		EMpFieldType Type = FGDALUtils::ToFieldType(FieldType, SubType);
		FieldValues[i] = FGDALUtils::ToFieldValue(Feature, i, Type);
	}

	// 2 convert geometry to geometry collection
	OGRGeometryCollection* GeometryCollection = FGDALUtils::ToGeometryCollection(Geometry);
	OGRGeometryCollection GeometryCollectionTemp;
	if (!GeometryCollection)
	{
		GeometryCollection = &GeometryCollectionTemp;
		GeometryCollection->addGeometryDirectly(Geometry);
	}

	// 3 get geometry points and return to receiver
	int NumGeometries = GeometryCollection->getNumGeometries();
	for (int i = 0; i < NumGeometries; i++)
	{
		OGRGeometry* poGeometry = GeometryCollection->getGeometryRef(i);
		EMpGeometryType InGeomType = FGDALUtils::ToGeomType(poGeometry->getGeometryType());
		TArray< TArray<FGeoPosition>> InPoints = FGDALUtils::ToGeomPoints(poGeometry);

		if (InPoints.Num() < 0)
		{
			continue;
		}
		UMpGraphics* Graphics = FGeometryUtils::CreateGraphics(InGeomType, FieldNames, FieldValues, InPoints[0]);
		if (Graphics)
		{
			Receiver->ReceiveGraphics(Graphics);
		}
	}

	return;
}

bool UMpGraphicsDataSource_GDAL::Load(TScriptInterface<IMpGraphicsDataReceiver> Receiver)
{

	const char* fileName = TCHAR_TO_ANSI(*FileName);
	const char* layerName = TCHAR_TO_ANSI(*Name);

	// 1 open data
	OGRLayer* poLayer = nullptr;
	GDALDataset* Dataset = FGDALUtils::OpenDataset(&poLayer, fileName, layerName);
	if (!poLayer)
	{
		return false;
	}

	// 2 load field names
	OGRFeatureDefn* poFDefn = poLayer->GetLayerDefn();
	int FieldCount = poFDefn->GetFieldCount();
	TArray<FName> FieldNames = FGDALUtils::GetFieldNames(poLayer);
	// map field names from data source to graphic property
	if (FieldMap.Num() > 0)
	{
		for (int i = 0; i < FieldCount; i++)
		{
			FName* MapName = FieldMap.Find(FieldNames[i]);
			if (MapName)
			{
				FieldNames[i] = *MapName;
			}
		}
	}

	// 3 set filters
	if (!AttributeFilter.IsEmpty())
	{
		const char* attributeFilter = TCHAR_TO_ANSI(*AttributeFilter);
		poLayer->SetAttributeFilter(attributeFilter);
	}

	// 4 read feature data, and return to receiver
	poLayer->ResetReading();
	while (OGRFeature* poFeature = poLayer->GetNextFeature())
	{
		LoadFeature(poFeature, FieldNames, Receiver);
	}

	// 5 close dataset
	FGDALUtils::CloseDataset(Dataset);
	return true;
}

AMpGraphicsDataSourceActor_GDAL::AMpGraphicsDataSourceActor_GDAL()
{
	DataSource = CreateDefaultSubobject<UMpGraphicsDataSource_GDAL>("DataSource");
}
