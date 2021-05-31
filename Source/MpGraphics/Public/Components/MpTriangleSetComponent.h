// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Components/MeshComponent.h"
#include "Materials/MaterialInterface.h"
#include "IndexTypes.h"

#include "MpTriangleSetComponent.generated.h"

USTRUCT()
struct FMpRenderableTriangleVertex
{
	GENERATED_BODY()

	FMpRenderableTriangleVertex()
		: Position(ForceInitToZero)
		, UV(ForceInitToZero)
		, Normal(ForceInitToZero)
		, Color(ForceInitToZero)
	{}

	FMpRenderableTriangleVertex( const FVector& InPosition, const FVector2D& InUV, const FVector& InNormal, const FColor& InColor )
		: Position( InPosition ),
		  UV( InUV ),
		  Normal( InNormal ),
		  Color( InColor )
	{
	}

	UPROPERTY()
	FVector Position;

	UPROPERTY()
	FVector2D UV;

	UPROPERTY()
	FVector Normal;

	UPROPERTY()
	FColor Color;
};


USTRUCT()
struct FMpRenderableTriangle
{
	GENERATED_BODY()

	FMpRenderableTriangle()
		: Material(nullptr)
		, Vertex0()
		, Vertex1()
		, Vertex2()
	{}

	FMpRenderableTriangle( UMaterialInterface* InMaterial, const FMpRenderableTriangleVertex& InVertex0, 
		const FMpRenderableTriangleVertex& InVertex1, const FMpRenderableTriangleVertex& InVertex2 )
		: Material( InMaterial ),
		  Vertex0( InVertex0 ),
		  Vertex1( InVertex1 ),
		  Vertex2( InVertex2 )
	{
	}

	UPROPERTY()
	UMaterialInterface* Material;

	UPROPERTY()
	FMpRenderableTriangleVertex Vertex0;

	UPROPERTY()
	FMpRenderableTriangleVertex Vertex1;

	UPROPERTY()
	FMpRenderableTriangleVertex Vertex2;
};

/**
* A component for rendering an arbitrary assortment of triangles. Suitable, for instance, for rendering highlighted faces.
*/
UCLASS()
class UMpTriangleSetComponent : public UMeshComponent
{
	GENERATED_BODY()

public:

	UMpTriangleSetComponent();

	/** Clear the triangle set */
	void Clear();

	/** Reserve enough memory for up to the given ID (for inserting via ID) */
	void ReserveTriangles(const int32 MaxID);

	/** Add a triangle to be rendered using the component. */
	int32 AddTriangle(const FMpRenderableTriangle& OverlayTriangle);

	/** Insert a triangle with the given ID to be rendered using the component. */
	void InsertTriangle(const int32 ID, const FMpRenderableTriangle& OverlayTriangle);

	/** Remove a triangle from the component. */
	void RemoveTriangle(const int32 ID);

	/** Queries whether a triangle with the given ID exists in the component. */
	bool IsTriangleValid(const int32 ID) const;


	/**
	 * Add a triangle with the given vertices, normal, Color, and Material
	 * @return ID of the triangle created
	 */
	int32 AddTriangle(const FVector& A, const FVector& B, const FVector& C, const FVector& Normal, const FColor& Color, UMaterialInterface* Material);

	/**
	 * Add a Quad (two triangles) with the given vertices, normal, Color, and Material
	 * @return ID of the two triangles created
	 */
	FIndex2i AddQuad(const FVector& A, const FVector& B, const FVector& C, const FVector& D, const FVector& Normal, const FColor& Color, UMaterialInterface* Material);

private:

	//~ Begin UPrimitiveComponent Interface.
	virtual FPrimitiveSceneProxy* CreateSceneProxy() override;
	//~ End UPrimitiveComponent Interface.

	//~ Begin UMeshComponent Interface.
	virtual int32 GetNumMaterials() const override;
	//~ End UMeshComponent Interface.

	//~ Begin USceneComponent Interface.
	virtual FBoxSphereBounds CalcBounds(const FTransform& LocalToWorld) const override;
	//~ End USceneComponent Interface.

	int32 FindOrAddMaterialIndex(UMaterialInterface* Material);

	UPROPERTY()
	mutable FBoxSphereBounds Bounds;

	UPROPERTY()
	mutable bool bBoundsDirty;

	TSparseArray<TTuple<int32, int32>> Triangles;
	TSparseArray<TSparseArray<FMpRenderableTriangle>> TrianglesByMaterial;
	TMap<UMaterialInterface*, int32> MaterialToIndex;

	friend class FMpTriangleSetSceneProxy;
};