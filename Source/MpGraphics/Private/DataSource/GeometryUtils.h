// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MpTypes.h"
#include "Misc/Variant.h"
#include "GeoPosition.h"

enum class EMpFieldType
{
	None = 0,
	String,
	Bool,
	Int32,
	Int64,
	Float,
	Double,
	DateTime,
	ByteArray
};

enum class EMpFieldSubtype
{
	None = 0,
	StringJson,
	StringXml
};

enum class EMpGeometryType
{
	None = 0,
	Point,
	LineString,
	Polygon
};

class UMpGraphics;

class FGeometryUtils
{
public:
	static UMpGraphics* CreateGraphics(EMpGeometryType GeometryType, const TArray<FName>& FieldNames, const TArray<FVariant>& FieldValues, const TArray<FGeoPosition>& Points);

	static UMpGraphics* CreatePolygonGraphics(const TArray<FName>& FieldNames, const TArray<FVariant>& FieldValues, const TArray<FGeoPosition>& Points);

	static UMpGraphics* CreatePolylineGraphics(const TArray<FName>& FieldNames, const TArray<FVariant>& FieldValues, const TArray<FGeoPosition>& Points);

	static UMpGraphics* CreatePointGraphics(const TArray<FName>& FieldNames, const TArray<FVariant>& FieldValues, const TArray<FGeoPosition>& Points);
};
