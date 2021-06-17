// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "VectorTypes.h"
#include "MatrixTypes.h"
#include "FrameTypes.h"

#include "GeoPosition.generated.h"

// 地理坐标，经度、续度、高度
USTRUCT()
struct MPBASE_API FGeoPosition 
{
	GENERATED_BODY()

	/**
	* @brief Creates a new instance.
	*/
	constexpr FGeoPosition() noexcept
	: Longitude(0.0), Latitude(0.0), Height(0.0) {}

	/**
	 * @brief Creates a new instance.
	 *
	 * @param longitudeRadians The longitude, in radians.
	 * @param latitudeRadians The latitude, in radians.
	 * @param heightMeters The height, in meters. Default value: 0.0.
	 */
	constexpr FGeoPosition(
		double LongitudeRadians,
		double LatitudeRadians,
		double HeightMeters = 0.0) noexcept
		: Longitude(LongitudeRadians),
		Latitude(LatitudeRadians),
		Height(HeightMeters) {}

	/**
	 * @brief Creates a new instance from a longitude and latitude specified in
	 * degrees, and a height given in meters.
	 *
	 * The values in the resulting object will be in radians.
	 *
	 * @param longitudeDegrees The longitude, in degrees.
	 * @param latitudeDegrees The latitude, in degrees.
	 * @param heightMeters The height, in meters. Default value: 0.0.
	 */
	static constexpr FGeoPosition FromDegrees(
		double LongitudeDegrees,
		double LatitudeDegrees,
		double HeightMeters = 0.0) noexcept {
		return FGeoPosition(
			FMath::DegreesToRadians(LongitudeDegrees),
			FMath::DegreesToRadians(LatitudeDegrees),
			HeightMeters);
	}

	void ToDegrees(double& LongitudeDegrees, double& LatitudeDegrees) const
	{
		LongitudeDegrees = FMath::RadiansToDegrees(Longitude);
		LatitudeDegrees = FMath::RadiansToDegrees(Latitude);
	}

	/**
	 * @brief The longitude, in radians.
	 */
	UPROPERTY(EditAnywhere, Category = "GeoTypes | Position")
	double Longitude;

	/**
	 * @brief The latitude, in radians.
	 */
	UPROPERTY(EditAnywhere, Category = "GeoTypes | Position")
	double Latitude;

	/**
	 * @brief The height, in meters.
	 */
	UPROPERTY(EditAnywhere, Category = "GeoTypes | Position")
	double Height;


	inline bool IsValid() const
	{
		return Longitude >= -PI && Longitude <= PI && Latitude >= -90 && Latitude <= 90;
	}

	inline constexpr bool operator == (const FGeoPosition& rhs) const noexcept
	{
		return Longitude == rhs.Longitude && Latitude == rhs.Latitude && Height == rhs.Height;
	}

	inline constexpr bool operator != (const FGeoPosition& rhs) const noexcept
	{
		return !(*this == rhs);
	}

	inline constexpr bool operator < (const FGeoPosition& rhs) const noexcept
	{
		if (Longitude == rhs.Longitude && Latitude == rhs.Latitude)
		{
			return Height < rhs.Height;
		}
		return (Longitude == rhs.Longitude && Latitude < rhs.Latitude) || Longitude < rhs.Longitude;
	}

	inline constexpr bool operator > (const FGeoPosition& rhs) const noexcept
	{
		return rhs < *this;
	}

	inline constexpr bool operator<=(const FGeoPosition& rhs) const noexcept
	{
		return !(*this > rhs);
	}

	inline constexpr bool operator>=(const FGeoPosition& rhs) const noexcept
	{
		return !(*this < rhs);
	}
};

USTRUCT()
struct MPBASE_API FGeoBoundingBox
{

	GENERATED_BODY()


	UPROPERTY(EditAnywhere, Category = "GeoTypes | BoundingBox")
	FGeoPosition MinLocation;
	UPROPERTY(EditAnywhere, Category = "GeoTypes | BoundingBox")
	FGeoPosition MaxLocation;

	FGeoBoundingBox() = default;
	FGeoBoundingBox(double minx, double miny, double maxx, double maxy) :
		MinLocation(minx, miny), MaxLocation(maxx, maxy) { }
	FGeoBoundingBox(const FGeoPosition& minLoc, const FGeoPosition& maxLoc) :
		MinLocation(minLoc), MaxLocation(maxLoc) { }

	bool IsValid() const
	{
		return MinLocation.IsValid() && MaxLocation.IsValid() && MinLocation <= MaxLocation;
	}

	bool Contains(const FGeoPosition& location) const
	{
		return location.Longitude >= MinLocation.Longitude && location.Latitude >= MinLocation.Latitude &&
			location.Longitude <= MaxLocation.Longitude && location.Latitude <= MaxLocation.Latitude;
	}

	FGeoBoundingBox& operator+=(const FGeoPosition &Location)
	{
		if (!Location.IsValid())
		{
			return *this;
		}
		if (IsValid())
		{
			MinLocation.Longitude = FMath::Min(MinLocation.Longitude, Location.Longitude);
			MinLocation.Latitude = FMath::Min(MinLocation.Latitude, Location.Latitude);
			MaxLocation.Longitude = FMath::Max(MaxLocation.Longitude, Location.Longitude);
			MaxLocation.Latitude = FMath::Max(MaxLocation.Latitude, Location.Latitude);

			MinLocation.Height = FMath::Min(MinLocation.Height, Location.Height);
			MaxLocation.Height = FMath::Max(MaxLocation.Height, Location.Height);
		}
		else
		{
			MinLocation = MaxLocation = Location;
		}
		return *this;
	}

	FGeoBoundingBox& operator+=(const FGeoBoundingBox &GeoBox)
	{
		if (!GeoBox.IsValid())
		{
			return *this;
		}
		if (IsValid())
		{
			MinLocation.Longitude = FMath::Min(MinLocation.Longitude, GeoBox.MinLocation.Longitude);
			MinLocation.Latitude = FMath::Min(MinLocation.Latitude, GeoBox.MinLocation.Latitude);
			MaxLocation.Longitude = FMath::Max(MaxLocation.Longitude, GeoBox.MaxLocation.Longitude);
			MaxLocation.Latitude = FMath::Max(MaxLocation.Latitude, GeoBox.MaxLocation.Latitude);
			MinLocation.Height = FMath::Min(MinLocation.Height, GeoBox.MinLocation.Height);
			MaxLocation.Height = FMath::Max(MaxLocation.Height, GeoBox.MaxLocation.Height);
		}
		else
		{
			*this = GeoBox;
		}
		return *this;
	}

	bool Intersect(const FGeoBoundingBox & Other) const
	{
		if ((MinLocation.Longitude > Other.MaxLocation.Longitude) || (Other.MinLocation.Longitude > MaxLocation.Longitude))
		{
			return false;
		}

		if ((MinLocation.Latitude > Other.MaxLocation.Latitude) || (Other.MinLocation.Latitude > MaxLocation.Latitude))
		{
			return false;
		}

		return true;
	}

	bool IsInside(const FGeoPosition & TestPoint) const
	{
		return ((TestPoint.Longitude > MinLocation.Longitude) && (TestPoint.Longitude < MaxLocation.Longitude) && (TestPoint.Latitude > MinLocation.Latitude) && (TestPoint.Latitude < MaxLocation.Latitude));
	}

	bool IsInside(const FGeoBoundingBox& Other) const
	{
		return (IsInside(Other.MinLocation) && IsInside(Other.MaxLocation));
	}

	FGeoPosition GetCenter() const
	{
		FGeoPosition Location;
		Location.Longitude = (MinLocation.Longitude + MaxLocation.Longitude) / 2;
		Location.Latitude = (MinLocation.Latitude + MaxLocation.Latitude) / 2;
		Location.Height = (MinLocation.Height + MaxLocation.Height) / 2;
		return Location;
	}

};


typedef FVector3d FGeoCartesian;
typedef FFrame3d FGeoFrame;
