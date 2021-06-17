#include "GDALUtils.h"

bool IsUTF8(const char* str)
{
	unsigned int nBytes = 0;
	unsigned char chr = *str;
	bool bAllAscii = true;

	for (unsigned int i = 0; str[i] != '\0'; ++i)
	{
		chr = *(str + i);
		if (nBytes == 0 && (chr & 0x80) != 0) {
			bAllAscii = false;
		}

		if (nBytes == 0) {
			if (chr >= 0x80) {

				if (chr >= 0xFC && chr <= 0xFD) {
					nBytes = 6;
				}
				else if (chr >= 0xF8) {
					nBytes = 5;
				}
				else if (chr >= 0xF0) {
					nBytes = 4;
				}
				else if (chr >= 0xE0) {
					nBytes = 3;
				}
				else if (chr >= 0xC0) {
					nBytes = 2;
				}
				else {
					return false;
				}

				nBytes--;
			}
		}
		else {
			if ((chr & 0xC0) != 0x80) {
				return false;
			}
			nBytes--;
		}
	}
	if (nBytes != 0) {
		return false;
	}

	if (bAllAscii) {
		return true;
	}

	return true;
}

GDALDataset* FGDALUtils::OpenDataset(OGRLayer** Layer, const char* fileName, const char* layerName)
{
	GDALDataset* Dataset = static_cast<GDALDataset*>(
		GDALOpenEx(fileName, GDAL_OF_VECTOR, NULL, NULL, NULL));
	if (!Dataset)
	{
		//GDAL::LogError("Open layer faild: " + LayerInfo.FileName);
		return nullptr;
	}

	*Layer = Dataset->GetLayerByName(layerName);
	if (!*Layer)
	{
		//GDAL::LogError("No layer in dataset : " + LayerInfo.Name);
		GDALClose(Dataset);
		return nullptr;
	}
	return Dataset;
}

void FGDALUtils::CloseDataset(GDALDataset* Dataset)
{
	GDALClose(Dataset);
}

TArray<FName> FGDALUtils::GetFieldNames(OGRLayer* poLayer)
{
	OGRFeatureDefn* poFDefn = poLayer->GetLayerDefn();
	int FieldCount = poFDefn->GetFieldCount();
	TArray<FName> FieldNames;
	FieldNames.SetNum(FieldCount);
	for (int i = 0; i < FieldCount; i++)
	{
		OGRFieldDefn* FieldDefn = poFDefn->GetFieldDefn(i);
		FieldNames[i] = ANSI_TO_TCHAR(FieldDefn->GetNameRef());
	}
	return FieldNames;
}

FString FGDALUtils::ToString(const char* str)
{
	FString OutString;
	if (!IsUTF8(str))
	{
		UE_LOG(LogTemp, Warning, TEXT("GDAL ToString failed, input is not encoded by UTF8."));
		OutString = "";
	}
	else
	{
		OutString = UTF8_TO_TCHAR(str);
	}
	return OutString;
}


EMpFieldType FGDALUtils::ToFieldType(OGRFieldType Type, OGRFieldSubType SubType)
{
	switch (Type)
	{
	case OFTInteger:
		if (SubType == OGRFieldSubType::OFSTBoolean)
			return EMpFieldType::Bool;
		else
			return EMpFieldType::Int32;
	case OFTInteger64:
		return EMpFieldType::Int64;
	case OFTReal:
		if (SubType == OGRFieldSubType::OFSTFloat32)
			return EMpFieldType::Float;
		else
			return EMpFieldType::Double;
	case OFTDateTime:
		return EMpFieldType::DateTime;
	case OFTBinary:
		return EMpFieldType::ByteArray;
	case OFTString:
		return EMpFieldType::String;
	default:
		return EMpFieldType::String;
	}
}

FVariant FGDALUtils::ToFieldValue(OGRFeature* Feature, int i, EMpFieldType Type)
{
	FVariant Value;
	switch (Type)
	{
	case EMpFieldType::Bool:
		Value = (bool)Feature->GetFieldAsInteger(i);
		break;
	case EMpFieldType::Int32:
		Value = (int32)Feature->GetFieldAsInteger(i);
		break;
	case EMpFieldType::Int64:
		Value = (int64)Feature->GetFieldAsInteger64(i);
		break;
	case EMpFieldType::Float:
		Value = (float)Feature->GetFieldAsDouble(i);
		break;
	case EMpFieldType::Double:
		Value = Feature->GetFieldAsDouble(i);
		break;
	case EMpFieldType::String:
		Value = FGDALUtils::ToString(Feature->GetFieldAsString(i));
		break;
	case EMpFieldType::DateTime:
	{
		int Year, Month, Day, Hour, Minute, Second, Flag;
		Feature->GetFieldAsDateTime(i, &Year, &Month, &Day, &Hour, &Minute, &Second, &Flag);
		Value = FDateTime(Year, Month, Day, Hour, Minute, Second);
		break;
	}
	case EMpFieldType::ByteArray:
	{
		int NumBytes;
		uint8* Bytes = Feature->GetFieldAsBinary(i, &NumBytes);
		Value = TArray<uint8>(Bytes, NumBytes);
	}
	default:
		Value = FGDALUtils::ToString(Feature->GetFieldAsString(i));
		break;
	}
	return Value;
}

EMpGeometryType FGDALUtils::ToGeomType(OGRwkbGeometryType Type)
{
	switch (Type)
	{
	case OGRwkbGeometryType::wkbUnknown:
		return EMpGeometryType::None;
	case OGRwkbGeometryType::wkbPoint:
		return EMpGeometryType::Point;
	case OGRwkbGeometryType::wkbLineString:
		return EMpGeometryType::LineString;
	case OGRwkbGeometryType::wkbPolygon:
		return EMpGeometryType::Polygon;
	case OGRwkbGeometryType::wkbMultiPoint:
		return EMpGeometryType::Point;
	case OGRwkbGeometryType::wkbMultiLineString:
		return EMpGeometryType::LineString;
	case OGRwkbGeometryType::wkbMultiPolygon:
		return EMpGeometryType::Polygon;
	default:
		return EMpGeometryType::None;
	}
}


TArray<FGeoPosition> PointToGeomPoints(OGRPoint* InPoint)
{
	TArray<FGeoPosition> Points;
	Points.SetNum(1);
	Points[0] = FGeoPosition::FromDegrees(InPoint->getX(), InPoint->getY(), InPoint->getZ());
	return Points;
}

TArray<FGeoPosition> LineToGeomPoints(OGRLineString* LineString)
{
	int NumPoints = LineString->getNumPoints();
	TArray<FGeoPosition> Points;
	Points.SetNum(NumPoints);
	for (int i = 0; i < NumPoints; i++)
	{
		Points[i] = FGeoPosition::FromDegrees(LineString->getX(i), LineString->getY(i), LineString->getZ(i));
	}
	return Points;
}

OGRGeometryCollection* FGDALUtils::ToGeometryCollection(OGRGeometry* Geometry)
{
	switch (Geometry->getGeometryType())
	{
	case wkbMultiPolygon:
	case wkbMultiLineString:
	case wkbMultiPoint:
		return Geometry->toGeometryCollection();
	default:
		return nullptr;
	}
}

TArray< TArray<FGeoPosition>> FGDALUtils::ToGeomPoints(OGRGeometry* Geometry)
{
	TArray< TArray<FGeoPosition>> Points;
	switch (Geometry->getGeometryType())
	{
	case wkbPolygon:
	{
		OGRPolygon* Polygon = Geometry->toPolygon();
		int NumInteriorRings = Polygon->getNumInteriorRings();
		Points.SetNum(NumInteriorRings + 1);
		Points[0] = LineToGeomPoints(Polygon->getExteriorRing());
		for (int i = 0; i < NumInteriorRings; i++)
		{
			Points[i + 1] = LineToGeomPoints(Polygon->getInteriorRing(i));
		}

		break;
	}
	case wkbLineString:
	{
		OGRLineString* LineString = Geometry->toLineString();
		Points.SetNum(1);
		Points[0] = LineToGeomPoints(LineString);
		break;
	}

	case wkbPoint:
	{
		OGRPoint* Point = Geometry->toPoint();
		Points.SetNum(1);
		Points[0] = PointToGeomPoints(Point);
		break;
	}
	default:
		break;
	}
	return Points;
}
