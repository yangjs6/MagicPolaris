#include "GeoEllipsoid.h"


const FGeoEllipsoid FGeoEllipsoid::WGS84(6378137.0, 6378137.0, 6356752.3142451793);

FGeoCartesian FGeoEllipsoid::GetSurfaceNormal(const FGeoCartesian& position) const noexcept {
	return (position * this->_oneOverRadiiSquared).Normalized();
}

FVector3d FGeoEllipsoid::GetSurfaceNormal(
	const FGeoPosition& cartographic) const noexcept 
{
	double longitude = cartographic.Longitude;
	double latitude = cartographic.Latitude;
	double cosLatitude = FMathd::Cos(latitude);

	return FVector3d(
		cosLatitude * FMathd::Cos(longitude),
		cosLatitude * FMathd::Sin(longitude),
		FMathd::Sin(latitude)).Normalized();
}

FVector3d FGeoEllipsoid::PositionToCartesian(
	const FGeoPosition& cartographic) const noexcept 
{
	FVector3d n = this->GetSurfaceNormal(cartographic);
	FVector3d k = this->_radiiSquared * n;
	double gamma = sqrt(n.Dot(k));
	k /= gamma;
	n *= cartographic.Height;
	return k + n;
}

FGeoPosition FGeoEllipsoid::CartesianToPosition(const FGeoCartesian& cartesian) const noexcept 
{
	FGeoCartesian p = this->ScaleToGeodeticSurface(cartesian);

	FVector3d n = this->GetSurfaceNormal(p);
	FVector3d h = cartesian - p;

	double longitude = FMathd::Atan2(n.Y, n.X);
	double latitude = asin(n.Z);
	double height = FMathd::Sign(h.Dot(cartesian)) * h.Length();

	return FGeoPosition(longitude, latitude, height);
}

FVector3d FGeoEllipsoid::ScaleToGeodeticSurface(const FVector3d& cartesian) const noexcept {
	double positionX = cartesian.X;
	double positionY = cartesian.Y;
	double positionZ = cartesian.Z;

	double oneOverRadiiX = this->_oneOverRadii.X;
	double oneOverRadiiY = this->_oneOverRadii.Y;
	double oneOverRadiiZ = this->_oneOverRadii.Z;

	double x2 = positionX * positionX * oneOverRadiiX * oneOverRadiiX;
	double y2 = positionY * positionY * oneOverRadiiY * oneOverRadiiY;
	double z2 = positionZ * positionZ * oneOverRadiiZ * oneOverRadiiZ;

	// Compute the squared FGeoEllipsoid norm.
	double squaredNorm = x2 + y2 + z2;
	double ratio = sqrt(1.0 / squaredNorm);

	// As an initial approximation, assume that the radial intersection is the
	// projection point.
	FVector3d intersection = cartesian * ratio;

	// If the position is near the center, the iteration will not converge.
	//if (squaredNorm < this->_centerToleranceSquared) {
	//	return !std::isfinite(ratio) ? std::optional<FVector3d>() : intersection;
	//}

	double oneOverRadiiSquaredX = this->_oneOverRadiiSquared.X;
	double oneOverRadiiSquaredY = this->_oneOverRadiiSquared.Y;
	double oneOverRadiiSquaredZ = this->_oneOverRadiiSquared.Z;

	// Use the gradient at the intersection point in place of the true unit
	// normal. The difference in magnitude will be absorbed in the multiplier.
	FVector3d gradient(
		intersection.X * oneOverRadiiSquaredX * 2.0,
		intersection.Y * oneOverRadiiSquaredY * 2.0,
		intersection.Z * oneOverRadiiSquaredZ * 2.0);

	// Compute the initial guess at the normal vector multiplier, lambda.
	double lambda =
		((1.0 - ratio) * cartesian.Length()) / (0.5 * gradient.Length());
	double correction = 0.0;

	double func;
	double denominator;
	double xMultiplier;
	double yMultiplier;
	double zMultiplier;
	double xMultiplier2;
	double yMultiplier2;
	double zMultiplier2;
	double xMultiplier3;
	double yMultiplier3;
	double zMultiplier3;

	do {
		lambda -= correction;

		xMultiplier = 1.0 / (1.0 + lambda * oneOverRadiiSquaredX);
		yMultiplier = 1.0 / (1.0 + lambda * oneOverRadiiSquaredY);
		zMultiplier = 1.0 / (1.0 + lambda * oneOverRadiiSquaredZ);

		xMultiplier2 = xMultiplier * xMultiplier;
		yMultiplier2 = yMultiplier * yMultiplier;
		zMultiplier2 = zMultiplier * zMultiplier;

		xMultiplier3 = xMultiplier2 * xMultiplier;
		yMultiplier3 = yMultiplier2 * yMultiplier;
		zMultiplier3 = zMultiplier2 * zMultiplier;

		func = x2 * xMultiplier2 + y2 * yMultiplier2 + z2 * zMultiplier2 - 1.0;

		// "denominator" here refers to the use of this expression in the velocity
		// and acceleration computations in the sections to follow.
		denominator = x2 * xMultiplier3 * oneOverRadiiSquaredX +
			y2 * yMultiplier3 * oneOverRadiiSquaredY +
			z2 * zMultiplier3 * oneOverRadiiSquaredZ;

		double derivative = -2.0 * denominator;

		correction = func / derivative;
	} while (abs(func) > 0.000000000001);

	return FVector3d(
		positionX * xMultiplier,
		positionY * yMultiplier,
		positionZ * zMultiplier);
}

