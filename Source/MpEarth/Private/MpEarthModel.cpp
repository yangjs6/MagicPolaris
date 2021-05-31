#include "MpEarthModel.h"

FMpEarthModel_Ellipoid::FMpEarthModel_Ellipoid(const FGeoPosition& InGeoreference)
	:Ellipoid(FGeoEllipsoid::WGS84.GetRadii())
{
	SetGeoreference(InGeoreference);
}

FMpEarthModel_Ellipoid::FMpEarthModel_Ellipoid(double a, double b, const FGeoPosition& InGeoreference)
	:Ellipoid(a, a, b)
{
	SetGeoreference(InGeoreference);
}

FGeoCartesian FMpEarthModel_Ellipoid::PositionToCartesian(const FGeoPosition& InPosition)
{
	return Ellipoid.PositionToCartesian(InPosition);
}

FGeoPosition FMpEarthModel_Ellipoid::CartesianToPosition(const FGeoCartesian& InCartesian)
{
	return Ellipoid.CartesianToPosition(InCartesian);
}

FGeoCartesian FMpEarthModel_Ellipoid::UECoordinateToCartesian(const FVector& InCoordinate)
{
	// TODO:
	FGeoCartesian InPoint(InCoordinate.X, InCoordinate.Y, InCoordinate.Z);
	return RefFrame.FromFramePoint(InPoint);
}

FVector FMpEarthModel_Ellipoid::CartesianToUECoordinate(const FGeoCartesian& InCartesian)
{
	// TODO:
	FGeoCartesian FramePoint = RefFrame.ToFramePoint(InCartesian);
	return FVector(FramePoint.X, FramePoint.Y, FramePoint.Z);
}

FVector FMpEarthModel_Ellipoid::PositionToUECoordinate(const FGeoPosition& InPosition)
{
	FGeoCartesian InCartesian = PositionToCartesian(InPosition);
	return CartesianToUECoordinate(InCartesian);
}

FGeoPosition FMpEarthModel_Ellipoid::UECoordinateToPosition(const FVector& InCoordinate)
{
	FGeoCartesian InCartesian = UECoordinateToCartesian(InCoordinate);
	return CartesianToPosition(InCartesian);
}

FGeoFrame FMpEarthModel_Ellipoid::ComputeFrameFromPosition(const FGeoPosition& InPosition)
{
	FGeoCartesian InCartesian = PositionToCartesian(InPosition);
	return ComputeFrameFromCartesian(InCartesian);
}

FGeoFrame FMpEarthModel_Ellipoid::ComputeFrameFromCartesian(const FGeoCartesian& InCartesian)
{
	FGeoCartesian Normal = Ellipoid.GetSurfaceNormal(InCartesian);
	return FGeoFrame(InCartesian, Normal);
}

void FMpEarthModel_Ellipoid::SetGeoreference(const FGeoPosition& InGeoreference)
{
	Georeference = InGeoreference;
	RefFrame = ComputeFrameFromPosition(InGeoreference);
}
