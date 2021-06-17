#include "MpEarthModel.h"

FVector3d ToUEVector2(100, -100, 100);
FVector ToUEVector(100, -100, 100);
FTransform ToUEScale(FQuat::Identity, FVector::ZeroVector, FVector(100, 100, 100));
FTransform ToUETransform(FQuat::Identity, FVector::ZeroVector, FVector(1, -1, 1));

UMpEarthModel_Ellipoid::UMpEarthModel_Ellipoid()
	:Ellipoid(FGeoEllipsoid::WGS84)
{

}

FGeoCartesian UMpEarthModel_Ellipoid::PositionToCartesian(const FGeoPosition& InPosition)
{
	return Ellipoid.PositionToCartesian(InPosition);
}

FGeoPosition UMpEarthModel_Ellipoid::CartesianToPosition(const FGeoCartesian& InCartesian)
{
	return Ellipoid.CartesianToPosition(InCartesian);
}

FGeoCartesian UMpEarthModel_Ellipoid::UECoordinateToCartesian(const FVector& InCoordinate)
{
	return UECoordinateToCartesian(InCoordinate, RefFrame);
}

FVector UMpEarthModel_Ellipoid::CartesianToUECoordinate(const FGeoCartesian& InCartesian)
{
	return CartesianToUECoordinate(InCartesian, RefFrame);
}

FGeoCartesian UMpEarthModel_Ellipoid::UECoordinateToCartesian(const FVector& InCoordinate, const FGeoFrame& LocalFrame)
{
	FVector Coordinate = InCoordinate / ToUEVector;
	FGeoCartesian InPoint(Coordinate.X, Coordinate.Y, Coordinate.Z);
	return LocalFrame.FromFramePoint(InPoint);
}

FVector UMpEarthModel_Ellipoid::CartesianToUECoordinate(const FGeoCartesian& InCartesian, const FGeoFrame& LocalFrame)
{
	FGeoCartesian FramePoint = LocalFrame.ToFramePoint(InCartesian);
	return FVector(FramePoint.X, FramePoint.Y, FramePoint.Z) * ToUEVector;
}

FGeoFrame UMpEarthModel_Ellipoid::ComputeFrameFromCartesian(const FGeoCartesian& InCartesian)
{
	FGeoCartesian Normal = Ellipoid.GetSurfaceNormal(InCartesian);
	return FGeoFrame(InCartesian, Normal);
}

FGeoFrame ToUEFrame(const FGeoFrame& InFrame)
{
	FVector3d AxesX, AxesY, AxesZ;
	InFrame.GetAxes(AxesX, AxesY, AxesZ);
	FGeoFrame OutFrame = FGeoFrame(InFrame.Origin * ToUEVector2, AxesX, -AxesY, AxesZ);
	return OutFrame;
}

FTransform UMpEarthModel_Ellipoid::ComputeUETransformFromFrame(const FGeoFrame& InFrame)
{
	FGeoFrame OutFrame;
	OutFrame.Rotation = RefFrame.ToFrame(InFrame.Rotation);
	OutFrame.Origin = RefFrame.ToFramePoint(InFrame.Origin);
	OutFrame = ToUEFrame(OutFrame);
	FTransform OutTransform = OutFrame.ToFTransform();
	return OutTransform ;
}

void UMpEarthModel_Ellipoid::UpdateFrame(const FGeoPosition& InPosition)
{
	RefFrame = ComputeFrameFromPosition(InPosition);
}


UMpEarthModel_Projection::UMpEarthModel_Projection()
{

}

FGeoCartesian UMpEarthModel_Projection::PositionToCartesian(const FGeoPosition& InPosition)
{
	return Projection.ProjectPosition(InPosition);
}

FGeoPosition UMpEarthModel_Projection::CartesianToPosition(const FGeoCartesian& InCartesian)
{
	return Projection.UnprojectPosition(InCartesian);
}

FGeoCartesian UMpEarthModel_Projection::UECoordinateToCartesian(const FVector& InCoordinate)
{
	FVector Coordinate = InCoordinate / ToUEVector;
	return FGeoCartesian(Coordinate.X + Offset.X, Coordinate.Y + Offset.Y, Coordinate.Z + Offset.Z);
}

FVector UMpEarthModel_Projection::CartesianToUECoordinate(const FGeoCartesian& InCartesian)
{
	return FVector(InCartesian.X - Offset.X, InCartesian.Y - Offset.Y, InCartesian.Z - Offset.Z) * ToUEVector;
}

FGeoCartesian UMpEarthModel_Projection::UECoordinateToCartesian(const FVector& InCoordinate, const FGeoFrame& LocalFrame)
{
	FVector Coordinate = InCoordinate / ToUEVector;
	FGeoCartesian InPoint(Coordinate.X, Coordinate.Y, Coordinate.Z);
	return LocalFrame.Origin + InPoint;
}

FVector UMpEarthModel_Projection::CartesianToUECoordinate(const FGeoCartesian& InCartesian, const FGeoFrame& LocalFrame)
{
	FGeoCartesian FramePoint = InCartesian - LocalFrame.Origin;
	return FVector(FramePoint.X, FramePoint.Y, FramePoint.Z) * ToUEVector;
}

FGeoFrame UMpEarthModel_Projection::ComputeFrameFromCartesian(const FGeoCartesian& InCartesian)
{
	FGeoCartesian Normal = FGeoCartesian::UnitZ();
	return FGeoFrame(InCartesian, Normal);
}

FTransform UMpEarthModel_Projection::ComputeUETransformFromFrame(const FGeoFrame& InFrame)
{
	FVector UECoordinate = CartesianToUECoordinate(InFrame.Origin);
	return FTransform(UECoordinate);
}

void UMpEarthModel_Projection::SetProjection(const FString& SourceSRS, const FString& TargetSRS)
{
	Projection.SetProjection(SourceSRS, TargetSRS);
}


