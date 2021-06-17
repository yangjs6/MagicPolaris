#include "Graphics/MpShapeGraphics.h"
#include "Components/MpTriangleSetComponent.h"
#include "Templates/SharedPointer.h"
#include "Generators/MeshShapeGenerator.h"

UMpShapeGraphics::UMpShapeGraphics()
{
	TriangleSet = CreateDefaultSubobject<UMpTriangleSet>("TriangleSet");

	TriangleSetComponent = CreateDefaultSubobject<UMpTriangleSetComponent>("TriangleSetComponent");
	TriangleSetComponent->SetMobility(EComponentMobility::Movable);
	TriangleSetComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriangleSetComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
}

template<class FVector3>
void Vector3ToVector(const FVector3& InV, FVector& OutV)
{
	OutV.X = InV.X;
	OutV.Y = InV.Y;
	OutV.Z = InV.Z;
}
template<class FVector2>
void Vector2ToVector(const FVector2& InV, FVector2D& OutV)
{
	OutV.X = InV.X;
	OutV.Y = InV.Y;
}

void MeshShapeToTriangleSet(TSharedPtr<FMeshShapeGenerator> InMeshShape, UMpTriangleSet* OutTriangleSet, const FColor& InColor)
{
	for (int i = 0; i < InMeshShape->Triangles.Num(); i++)
	{
		const FIndex3i& Tri = InMeshShape->Triangles[i];
		const FIndex3i& TriN = InMeshShape->TriangleNormals[i];
		const FIndex3i& TriUV = InMeshShape->TriangleUVs[i];
		int PolygonID = InMeshShape->TrianglePolygonIDs[i];

		 
		FMpRenderableTriangleVertex TriangleVertexA, TriangleVertexB, TriangleVertexC;

		// Vertex A
		Vector3ToVector(InMeshShape->Vertices[Tri.A], TriangleVertexA.Position);
		Vector3ToVector(InMeshShape->Normals[TriN.A], TriangleVertexA.Normal);
		Vector2ToVector(InMeshShape->UVs[TriUV.A], TriangleVertexA.UV);
		TriangleVertexA.Color = InColor;

		// Vertex B
		Vector3ToVector(InMeshShape->Vertices[Tri.B], TriangleVertexB.Position);
		Vector3ToVector(InMeshShape->Normals[TriN.B], TriangleVertexB.Normal);
		Vector2ToVector(InMeshShape->UVs[TriUV.B], TriangleVertexB.UV);
		TriangleVertexB.Color = InColor;

		// Vertex C
		Vector3ToVector(InMeshShape->Vertices[Tri.C], TriangleVertexC.Position);
		Vector3ToVector(InMeshShape->Normals[TriN.C], TriangleVertexC.Normal);
		Vector2ToVector(InMeshShape->UVs[TriUV.C], TriangleVertexC.UV);
		TriangleVertexC.Color = InColor;

		OutTriangleSet->AddTriangle(FMpRenderableTriangle(PolygonID, TriangleVertexA, TriangleVertexB, TriangleVertexC));
	}
}

void UMpShapeGraphics::UpdateGeometry(IMpSceneView* pView, IMpGraphicsDrawContext* DrawContext, float DeltaTime)
{
	TriangleSet->Clear();
	if (bFill)
	{
		TSharedPtr<FMeshShapeGenerator> FillShape = GenarateFillShape();
		MeshShapeToTriangleSet(FillShape, TriangleSet, FillColor);
	}

	TriangleSetComponent->SetTriangleSet(TriangleSet);

	if (bOutline)
	{
		GenarateOutlineShape();
	}
}

void UMpShapeGraphics::UpdateMaterial(IMpSceneView* pView, IMpGraphicsDrawContext* DrawContext, float DeltaTime)
{
	UMaterialInterface* DefaultMaterial = DrawContext->GetDefaultMaterial();
	IMpGraphicsStyle* MaterialStyle = DrawContext->GetStyle(FillStyle);
	for (auto MaterialToIndex : TriangleSet->MaterialToIndex)
	{
		UMaterialInterface* Material = MaterialStyle ? MaterialStyle->GetMaterialById(MaterialToIndex.Key) : nullptr;
		TriangleSetComponent->SetMaterial(MaterialToIndex.Value, Material ? Material : DefaultMaterial);
	}
}

void UMpShapeGraphics::RegisterToDrawContext(IMpSceneView* pView, IMpGraphicsDrawContext* DrawContext)
{
	DrawContext->RegisterDrawComponent(TriangleSetComponent);
}

void UMpShapeGraphics::UnRegisterFromDrawContext(IMpSceneView* pView, IMpGraphicsDrawContext* DrawContext)
{
	DrawContext->UnRegisterDrawComponent(TriangleSetComponent);
}

void UMpShapeGraphicsPoint::UpdateBoundingBox()
{
	BoundingBox.MinLocation = BoundingBox.MaxLocation = Position;
}

void UMpShapeGraphicsPoint::UpdatePosition(IMpSceneView* pView, IMpGraphicsDrawContext* DrawContext, float DeltaTime)
{
	TScriptInterface<IMpEarthModel> EarthModel = pView->GetEarthModel();

	LocalFrame = EarthModel->ComputeFrameFromPosition(Position);

	UECoordinate = EarthModel->PositionToUECoordinate(Position, LocalFrame);

	FTransform RelativeTransform = EarthModel->ComputeUETransformFromFrame(LocalFrame);

	TriangleSetComponent->SetRelativeTransform(RelativeTransform); 
}

void UMpShapeGraphicsPoints::UpdateBoundingBox()
{
	if (Positions.Num() <= 0)
	{
		BoundingBox = FGeoBoundingBox();
	}
	else
	{
		BoundingBox.MinLocation = BoundingBox.MaxLocation = Positions[0];
		for (int i = 1; i < Positions.Num(); i++)
		{
			BoundingBox += Positions[i];
		}
	}
}

void UMpShapeGraphicsPoints::UpdatePosition(IMpSceneView* pView, IMpGraphicsDrawContext* DrawContext, float DeltaTime)
{
	TScriptInterface<IMpEarthModel> EarthModel = pView->GetEarthModel();

	LocalFrame = EarthModel->ComputeFrameFromPosition(GetBoundingBox().GetCenter());

	UECoordinates.SetNum(Positions.Num());
	for (int i = 0; i < Positions.Num(); i++)
	{
		UECoordinates[i] = EarthModel->PositionToUECoordinate(Positions[i], LocalFrame);
	}

	FTransform RelativeTransform = EarthModel->ComputeUETransformFromFrame(LocalFrame);

	TriangleSetComponent->SetRelativeTransform(RelativeTransform);
}
