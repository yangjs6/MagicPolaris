// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GeoPosition.h"

/**
 * @brief A quadratic surface defined in Cartesian coordinates.
 *
 * The surface is defined by the equation `(x / a)^2 + (y / b)^2 + (z / c)^2 =
 * 1`. This is primarily used by Cesium to represent the shape of planetary
 * bodies. Rather than constructing this object directly, one of the provided
 * constants is normally used.
 */
class MPBASE_API FGeoEllipsoid final
{
public:
    /**
     * @brief An FGeoEllipsoid instance initialized to the WGS84 standard.
     *
     * The FGeoEllipsoid is initialized to the  World Geodetic System (WGS84)
     * standard, as defined in
     * https://earth-info.nga.mil/GandG/publications/tr8350.2/wgs84fin.pdf.
     */
    static /*constexpr*/ const FGeoEllipsoid WGS84;

    /**
     * @brief Creates a new instance.
     *
     * @param x The radius in x-direction.
     * @param y The radius in y-direction.
     * @param z The radius in z-direction.
     */
    constexpr FGeoEllipsoid(double x, double y, double z) noexcept
        : FGeoEllipsoid(FVector3d(x, y, z)) {}

    /**
     * @brief Creates a new instance.
     *
     * @param radii The radii in x-, y-, and z-direction.
     */
    constexpr FGeoEllipsoid(const FVector3d& radii) noexcept
        : _radii(radii),
        _radiiSquared(radii * radii),
        _oneOverRadii( FVector3d::One() / radii),
        _oneOverRadiiSquared(FVector3d::One() / _radiiSquared) {}

    /**
     * @brief Returns the radii in x-, y-, and z-direction.
     */
    constexpr const FVector3d& GetRadii() const noexcept { return this->_radii; }

    /**
     * @brief Computes the normal of the plane tangent to the surface of the
     * FGeoEllipsoid at the provided position.
     *
     * @param position The cartesian position for which to to determine the
     * surface normal.
     * @return The normal.
     */
    FGeoCartesian GetSurfaceNormal(const FGeoCartesian& position) const noexcept;

    /**
     * @brief Computes the normal of the plane tangent to the surface of the
     * FGeoEllipsoid at the provided position.
     *
     * @param cartographic The {@link FGeoPosition} position for which to to
     * determine the surface normal.
     * @return The normal.
     */
    FGeoCartesian GetSurfaceNormal(const FGeoPosition& cartographic) const noexcept;

    /**
     * @brief Converts the provided {@link FGeoPosition} to cartesian
     * representation.
     *
     * @param cartographic The {@link FGeoPosition} position.
     * @return The cartesian representation.
     */
    FGeoCartesian PositionToCartesian(const FGeoPosition& cartographic) const noexcept;

    /**
     * @brief Converts the provided cartesian to a {@link FGeoPosition}
     * representation.
     *
     * The result will be the empty optional if the given cartesian is at the
     * center of this FGeoEllipsoid.
     *
     * @param cartesian The cartesian position.
     * @return The {@link FGeoPosition} representation, or the empty optional if
     * the cartesian is at the center of this FGeoEllipsoid.
     */
    FGeoPosition CartesianToPosition(const FGeoCartesian& cartesian) const noexcept;

    /**
     * @brief Scales the given cartesian position along the geodetic surface
     * normal so that it is on the surface of this FGeoEllipsoid.
     *
     * The result will be the empty optional if the position is at the center of
     * this FGeoEllipsoid.
     *
     * @param cartesian The cartesian position.
     * @return The scaled position, or the empty optional if
     * the cartesian is at the center of this FGeoEllipsoid.
     */
    FGeoCartesian ScaleToGeodeticSurface(const FGeoCartesian& cartesian) const noexcept;

    /**
     * @brief The maximum radius in any dimension.
     *
     * @return The maximum radius.
     */
    constexpr double GetMaximumRadius() const noexcept {
        return this->_radii.MaxElement();
    }

    /**
     * @brief The minimum radius in any dimension.
     *
     * @return The minimum radius.
     */
	constexpr double GetMinimumRadius() const noexcept {
		return this->_radii.MinElement();
    }

    /**
     * @brief Returns `true` if two elliposids are equal.
     */
    constexpr bool operator==(const FGeoEllipsoid& rhs) const noexcept {
        return this->_radii == rhs._radii;
    };

    /**
     * @brief Returns `true` if two elliposids are *not* equal.
     */
    constexpr bool operator!=(const FGeoEllipsoid& rhs) const noexcept {
        return !(*this == rhs);
    };

private:
    FVector3d _radii;
    FVector3d _radiiSquared;
    FVector3d _oneOverRadii;
    FVector3d _oneOverRadiiSquared;
};