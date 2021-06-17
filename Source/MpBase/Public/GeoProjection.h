#pragma once

#include "CoreMinimal.h"
#include "GeoPosition.h"

class OGRSpatialReference;
class OGRCoordinateTransformation;

// transforming between coordinate systems.
class MPBASE_API FGeoProjection
{
public:
	FGeoProjection();
	FGeoProjection(const FString& SourceSRS, const FString& TargetSRS);
	virtual ~FGeoProjection();

    void SetProjection(const FString& SourceSRS, const FString& TargetSRS);

    /**
     * @brief Converts the provided {@link FGeoPosition} to cartesian
     * representation.
     *
     * @param cartographic The {@link FGeoPosition} position.
     * @return The cartesian representation.
     */
    virtual FGeoCartesian ProjectPosition(const FGeoPosition& cartographic) const noexcept;

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
    virtual FGeoPosition UnprojectPosition(const FGeoCartesian& cartesian) const noexcept;

protected:
    OGRCoordinateTransformation* OGRTransformationPtr; ///< impl using ogr/gdal/proj handle
    OGRCoordinateTransformation* OGRTransformationInvPtr; ///< impl using ogr/gdal/proj handle
};
