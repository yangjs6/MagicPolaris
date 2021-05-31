// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MpEntity.generated.h"


/*
 * Basic object type that can be rendered in the 3D scene
*/
UCLASS(Abstract)
class MPCORE_API UMpEntity : public UObject
{
	GENERATED_BODY()
public:
	virtual bool IsShow() const PURE_VIRTUAL(, return true;);
	virtual FVector GetPosition() const PURE_VIRTUAL(, return FVector::ZeroVector;);
	virtual FBox2D GetScreenBoundingBox(const FVector2D& ScreenPosition, const int PixelRange) const PURE_VIRTUAL(, return FBox2D(););

	virtual void SetClusterShow(bool bClusterShow) PURE_VIRTUAL(,);
	virtual void UpdateProperty(float Time) PURE_VIRTUAL(, );
};

/*
 * A collection of Entities.
 * Entities are added and removed from the collection.
*/
UCLASS()
class MPCORE_API UMpEntityCollection : public UObject
{
	GENERATED_BODY()
public:
	// Adds a Entity to the collection.
	void Add(UMpEntity* Entity) { Entities.Add(Entity); }

	// Removes a Entity from the collection.
	void Remove(UMpEntity* Entity) { Entities.Remove(Entity); }

	// Removes all Entities from the collection.
	void RemoveAll() { Entities.Empty(); }

	// Check whether this collection contains a given Entity.
	bool Contains(UMpEntity* Entity) const { return Entities.Contains(Entity); }

	// Returns the number of Entities in this collection.
	int GetNum() const { return Entities.Num(); }

	// Returns the Entity in the collection at the specified index.
	UMpEntity* GetEntity(int Index) const { return Entities[Index]; }

protected:
	UPROPERTY()
	TArray<UMpEntity*> Entities;
};
