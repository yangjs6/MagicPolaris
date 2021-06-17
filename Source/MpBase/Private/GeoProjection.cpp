#include "GeoProjection.h"
#include "gdal_priv.h"

FGeoProjection::FGeoProjection()
	:OGRTransformationPtr(nullptr), OGRTransformationInvPtr(nullptr)
{

}
FGeoProjection::FGeoProjection(const FString& SourceSRS, const FString& TargetSRS)
	:OGRTransformationPtr(nullptr), OGRTransformationInvPtr(nullptr)
{
	SetProjection(SourceSRS, TargetSRS);
}

FGeoProjection::~FGeoProjection()
{
	if (OGRTransformationPtr)
	{
		OGRCoordinateTransformation::DestroyCT((OGRCoordinateTransformation*)OGRTransformationPtr);
	}
	if (OGRTransformationInvPtr)
	{
		OGRCoordinateTransformation::DestroyCT((OGRCoordinateTransformation*)OGRTransformationInvPtr);
	}
}

void FGeoProjection::SetProjection(const FString& SourceSRS, const FString& TargetSRS)
{
	if (OGRTransformationPtr)
	{
		OGRCoordinateTransformation::DestroyCT((OGRCoordinateTransformation*)OGRTransformationPtr);
	}
	if (OGRTransformationInvPtr)
	{
		OGRCoordinateTransformation::DestroyCT((OGRCoordinateTransformation*)OGRTransformationInvPtr);
	}

	OGRSpatialReference* oSourceSRS = new OGRSpatialReference();
	OGRSpatialReference* oTargetSRS = new OGRSpatialReference();

	const char* SourceSRSProj4 = TCHAR_TO_ANSI(*SourceSRS);
	if (oSourceSRS->importFromProj4(SourceSRSProj4) != OGRERR_NONE)
	{
		delete oSourceSRS;
		delete oTargetSRS;
		return;
	}

	const char* TargetSRSProj4 = TCHAR_TO_ANSI(*TargetSRS);
	if (oTargetSRS->importFromProj4(TargetSRSProj4) != OGRERR_NONE)
	{
		delete oSourceSRS;
		delete oTargetSRS;
		return;
	}

	OGRTransformationPtr = OGRCreateCoordinateTransformation(oSourceSRS, oTargetSRS);
	OGRTransformationInvPtr = OGRCreateCoordinateTransformation(oTargetSRS, oSourceSRS);
}

FGeoCartesian FGeoProjection::ProjectPosition(const FGeoPosition& Position) const noexcept
{
	FGeoCartesian Cartesion;
	Position.ToDegrees(Cartesion.X, Cartesion.Y);
	Cartesion.Z = Position.Height;

	OGRCoordinateTransformation* oTransformer = StaticCast<OGRCoordinateTransformation*>(OGRTransformationPtr);
	if (oTransformer)
	{
		oTransformer->Transform(1, &Cartesion.X, &Cartesion.Y);
	}
	return Cartesion;
}

FGeoPosition FGeoProjection::UnprojectPosition(const FGeoCartesian& Cartesian) const noexcept
{
	FGeoPosition Position;
	OGRCoordinateTransformation* oTransformer = StaticCast<OGRCoordinateTransformation*>(OGRTransformationInvPtr);
	if (oTransformer)
	{
		double X = Cartesian.X;
		double Y = Cartesian.Y;
		if (oTransformer->Transform(1, &X, &Y))
		{
			Position = FGeoPosition::FromDegrees(X, Y, Cartesian.Z);
		}
	}
	return Position;
}
