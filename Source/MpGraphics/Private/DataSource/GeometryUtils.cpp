#include "GeometryUtils.h"
#include "Graphics/MpPolygonGraphics.h"
#include "Graphics/MpPolylineGraphics.h"

UMpGraphics* FGeometryUtils::CreateGraphics(EMpGeometryType GeometryType, const TArray<FName>& FieldNames, const TArray<FVariant>& FieldValues, const TArray<FGeoPosition>& Points)
{
	switch (GeometryType)
	{
	case EMpGeometryType::Polygon:
		return CreatePolygonGraphics(FieldNames, FieldValues, Points);
		break;
	case EMpGeometryType::LineString:
		return CreatePolylineGraphics(FieldNames, FieldValues, Points);
		break;
	case EMpGeometryType::Point:
		return CreatePointGraphics(FieldNames, FieldValues, Points);
		break;
	}
	return nullptr;
}

template<typename T>
void GetFieldValue(const TArray<FName>& FieldNames, const TArray<FVariant>& FieldValues, const FName& FieldName, T& FieldValue)
{
	int FieldIndex = FieldNames.Find(FieldName);
	if (FieldIndex > 0)
	{
		FieldValue = FieldValues[FieldIndex].GetValue<T>();
	}
}

UMpGraphics* FGeometryUtils::CreatePolygonGraphics(const TArray<FName>& FieldNames, const TArray<FVariant>& FieldValues, const TArray<FGeoPosition>& Points)
{
	int NumPoints = Points.Num();
	if (NumPoints < 3)
	{
		return nullptr;
	}

	UMpPolygonGraphics* Graphics = NewObject<UMpPolygonGraphics>();

	const FName& HeightFiled = "Height";

	GetFieldValue(FieldNames, FieldValues, HeightFiled, Graphics->ExtrudedHeight);

	// TODO: 数据源和UE的左右手系不一致，点序需要转置
	Graphics->Positions.SetNum(NumPoints);
	for (int i = 0; i < NumPoints; i++)
	{
		Graphics->Positions[i] = Points[NumPoints -1 - i];
	}
	// TODO: 首尾点重复的情况
	if (Points[0] == Points[NumPoints -1])
	{
		Graphics->Positions.SetNum(NumPoints - 1);
	}

	return Graphics;
}

UMpGraphics* FGeometryUtils::CreatePolylineGraphics(const TArray<FName>& FieldNames, const TArray<FVariant>& FieldValues, const TArray<FGeoPosition>& Points)
{
	return nullptr;
}

UMpGraphics* FGeometryUtils::CreatePointGraphics(const TArray<FName>& FieldNames, const TArray<FVariant>& FieldValues, const TArray<FGeoPosition>& Points)
{
	return nullptr;
}

