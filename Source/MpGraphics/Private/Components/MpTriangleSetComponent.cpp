// Copyright Epic Games, Inc. All Rights Reserved.

#include "Components/MpTriangleSetComponent.h"

#include "DynamicMeshBuilder.h"
#include "Engine/CollisionProfile.h"
#include "Materials/MaterialRelevance.h"
#include "LocalVertexFactory.h"
#include "PrimitiveSceneProxy.h"
#include "StaticMeshResources.h"

#include "Algo/Accumulate.h"

struct FMpTriangleSetMeshBatchData
{
	FMpTriangleSetMeshBatchData()
		: MaterialProxy(nullptr)
	{}

	FMaterialRenderProxy* MaterialProxy;
	int32 StartIndex;
	int32 NumPrimitives;
	int32 MinVertexIndex;
	int32 MaxVertexIndex;
};

/** Class for the MpTriangleSetComponent data passed to the render thread. */
class FMpTriangleSetSceneProxy final : public FPrimitiveSceneProxy
{
public:

	FMpTriangleSetSceneProxy(UMpTriangleSetComponent* Component)
		: FPrimitiveSceneProxy(Component),
		MaterialRelevance(Component->GetMaterialRelevance(GetScene().GetFeatureLevel())),
		VertexFactory(GetScene().GetFeatureLevel(), "FPointSetSceneProxy")
	{
		const int32 NumTriangleVertices = Algo::Accumulate(Component->TriangleSet->TrianglesByMaterial, 0, [](int32 Acc, const TSparseArray<FMpRenderableTriangle>& Tris) { return Acc + Tris.Num() * 3; });
		const int32 NumTriangleIndices = NumTriangleVertices;

		const int32 NumTextureCoordinates = 1;

		VertexBuffers.PositionVertexBuffer.Init(NumTriangleVertices);
		VertexBuffers.StaticMeshVertexBuffer.Init(NumTriangleVertices, NumTextureCoordinates);
		VertexBuffers.ColorVertexBuffer.Init(NumTriangleVertices);
		IndexBuffer.Indices.SetNumUninitialized(NumTriangleIndices);

		int32 VertexBufferIndex = 0;
		int32 IndexBufferIndex = 0;

		// Triangles
		int32 MaterialIndex = 0;
		for (const auto& MaterialTriangles : Component->TriangleSet->TrianglesByMaterial)
		{
			MeshBatchDatas.Emplace();
			FMpTriangleSetMeshBatchData& MeshBatchData = MeshBatchDatas.Last();
			MeshBatchData.MinVertexIndex = VertexBufferIndex;
			MeshBatchData.MaxVertexIndex = VertexBufferIndex + MaterialTriangles.Num() * 3 - 1;
			MeshBatchData.StartIndex = IndexBufferIndex;
			MeshBatchData.NumPrimitives = MaterialTriangles.Num();
			UMaterialInterface* Material = Component->GetMaterial(MaterialIndex);
			if (Material)
			{
				MeshBatchData.MaterialProxy = Material->GetRenderProxy();
			}

			for (const FMpRenderableTriangle& Triangle : MaterialTriangles)
			{
				VertexBuffers.PositionVertexBuffer.VertexPosition(VertexBufferIndex + 0) = Triangle.Vertex0.Position;
				VertexBuffers.PositionVertexBuffer.VertexPosition(VertexBufferIndex + 1) = Triangle.Vertex1.Position;
				VertexBuffers.PositionVertexBuffer.VertexPosition(VertexBufferIndex + 2) = Triangle.Vertex2.Position;

				VertexBuffers.StaticMeshVertexBuffer.SetVertexTangents(VertexBufferIndex + 0, FVector(1, 0, 0), FVector(0, 1, 0), Triangle.Vertex0.Normal);
				VertexBuffers.StaticMeshVertexBuffer.SetVertexTangents(VertexBufferIndex + 1, FVector(1, 0, 0), FVector(0, 1, 0), Triangle.Vertex1.Normal);
				VertexBuffers.StaticMeshVertexBuffer.SetVertexTangents(VertexBufferIndex + 2, FVector(1, 0, 0), FVector(0, 1, 0), Triangle.Vertex2.Normal);

				VertexBuffers.StaticMeshVertexBuffer.SetVertexUV(VertexBufferIndex + 0, 0, Triangle.Vertex0.UV);
				VertexBuffers.StaticMeshVertexBuffer.SetVertexUV(VertexBufferIndex + 1, 0, Triangle.Vertex1.UV);
				VertexBuffers.StaticMeshVertexBuffer.SetVertexUV(VertexBufferIndex + 2, 0, Triangle.Vertex2.UV);

				VertexBuffers.ColorVertexBuffer.VertexColor(VertexBufferIndex + 0) = Triangle.Vertex0.Color;
				VertexBuffers.ColorVertexBuffer.VertexColor(VertexBufferIndex + 1) = Triangle.Vertex1.Color;
				VertexBuffers.ColorVertexBuffer.VertexColor(VertexBufferIndex + 2) = Triangle.Vertex2.Color;

				IndexBuffer.Indices[IndexBufferIndex + 0] = VertexBufferIndex + 0;
				IndexBuffer.Indices[IndexBufferIndex + 1] = VertexBufferIndex + 1;
				IndexBuffer.Indices[IndexBufferIndex + 2] = VertexBufferIndex + 2;

				VertexBufferIndex += 3;
				IndexBufferIndex += 3;
			}

			MaterialIndex++;
		}

		ENQUEUE_RENDER_COMMAND(LineSetVertexBuffersInit)(
			[this](FRHICommandListImmediate& RHICmdList)
			{
				VertexBuffers.PositionVertexBuffer.InitResource();
				VertexBuffers.StaticMeshVertexBuffer.InitResource();
				VertexBuffers.ColorVertexBuffer.InitResource();

				FLocalVertexFactory::FDataType Data;
				VertexBuffers.PositionVertexBuffer.BindPositionVertexBuffer(&VertexFactory, Data);
				VertexBuffers.StaticMeshVertexBuffer.BindTangentVertexBuffer(&VertexFactory, Data);
				VertexBuffers.StaticMeshVertexBuffer.BindTexCoordVertexBuffer(&VertexFactory, Data);
				VertexBuffers.ColorVertexBuffer.BindColorVertexBuffer(&VertexFactory, Data);
				VertexFactory.SetData(Data);

				VertexFactory.InitResource();
				IndexBuffer.InitResource();
			});
	}

	virtual ~FMpTriangleSetSceneProxy()
	{
		VertexBuffers.PositionVertexBuffer.ReleaseResource();
		VertexBuffers.StaticMeshVertexBuffer.ReleaseResource();
		VertexBuffers.ColorVertexBuffer.ReleaseResource();
		IndexBuffer.ReleaseResource();
		VertexFactory.ReleaseResource();
	}

	virtual void GetDynamicMeshElements(const TArray<const FSceneView*>& Views,
		const FSceneViewFamily& ViewFamily, uint32 VisibilityMap, FMeshElementCollector& Collector) const override
	{
		QUICK_SCOPE_CYCLE_COUNTER(STAT_OverlaySceneProxy_GetDynamicMeshElements);

		for (int32 ViewIndex = 0; ViewIndex < Views.Num(); ViewIndex++)
		{
			if (VisibilityMap & (1 << ViewIndex))
			{
				for (const FMpTriangleSetMeshBatchData& MeshBatchData : MeshBatchDatas)
				{
					const FSceneView* View = Views[ViewIndex];
					FMeshBatch& Mesh = Collector.AllocateMesh();
					FMeshBatchElement& BatchElement = Mesh.Elements[0];
					BatchElement.IndexBuffer = &IndexBuffer;
					Mesh.bWireframe = false;
					Mesh.VertexFactory = &VertexFactory;
					Mesh.MaterialRenderProxy = MeshBatchData.MaterialProxy;

					FDynamicPrimitiveUniformBuffer& DynamicPrimitiveUniformBuffer = Collector.AllocateOneFrameResource<FDynamicPrimitiveUniformBuffer>();
					DynamicPrimitiveUniformBuffer.Set(GetLocalToWorld(), GetLocalToWorld(), GetBounds(), GetLocalBounds(), true, false, DrawsVelocity(), false);
					BatchElement.PrimitiveUniformBufferResource = &DynamicPrimitiveUniformBuffer.UniformBuffer;

					BatchElement.FirstIndex = MeshBatchData.StartIndex;
					BatchElement.NumPrimitives = MeshBatchData.NumPrimitives;
					BatchElement.MinVertexIndex = MeshBatchData.MinVertexIndex;
					BatchElement.MaxVertexIndex = MeshBatchData.MaxVertexIndex;
					Mesh.ReverseCulling = IsLocalToWorldDeterminantNegative();
					Mesh.Type = PT_TriangleList;
					Mesh.DepthPriorityGroup = SDPG_World;
					Mesh.bCanApplyViewModeOverrides = false;
					Collector.AddMesh(ViewIndex, Mesh);
				}
			}
		}
	}

	virtual FPrimitiveViewRelevance GetViewRelevance(const FSceneView* View) const override
	{
		FPrimitiveViewRelevance Result;
		Result.bDrawRelevance = IsShown(View);
		Result.bShadowRelevance = IsShadowCast(View);
		Result.bDynamicRelevance = true;
		Result.bRenderInMainPass = ShouldRenderInMainPass();
		Result.bUsesLightingChannels = GetLightingChannelMask() != GetDefaultLightingChannelMask();
		Result.bRenderCustomDepth = ShouldRenderCustomDepth();
		Result.bTranslucentSelfShadow = bCastVolumetricTranslucentShadow;
		MaterialRelevance.SetPrimitiveViewRelevance(Result);
		Result.bVelocityRelevance = IsMovable() && Result.bOpaque && Result.bRenderInMainPass;
		return Result;
	}

	virtual bool CanBeOccluded() const override
	{
		return !MaterialRelevance.bDisableDepthTest;
	}

	virtual uint32 GetMemoryFootprint() const override { return sizeof(*this) + GetAllocatedSize(); }

	uint32 GetAllocatedSize() const { return FPrimitiveSceneProxy::GetAllocatedSize(); }

	virtual SIZE_T GetTypeHash() const override
	{
		static SIZE_T UniquePointer;
		return reinterpret_cast<SIZE_T>(&UniquePointer);
	}

private:
	TArray<FMpTriangleSetMeshBatchData> MeshBatchDatas;
	FMaterialRelevance MaterialRelevance;
	FLocalVertexFactory VertexFactory;
	FStaticMeshVertexBuffers VertexBuffers;
	FDynamicMeshIndexBuffer32 IndexBuffer;
};


UMpTriangleSetComponent::UMpTriangleSetComponent()
{
	CastShadow = false;
	bSelectable = false;
	PrimaryComponentTick.bCanEverTick = false;
	bBoundsDirty = true;

	SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
}

void UMpTriangleSetComponent::SetTriangleSet(UMpTriangleSet* InTriangleSet)
{
	TriangleSet = InTriangleSet;

	MarkRenderStateDirty();
	bBoundsDirty = true;
}

int32 UMpTriangleSet::FindOrAddMaterialIndex(const int32& Material)
{
	const int32* MaterialIndexPtr = MaterialToIndex.Find(Material);
	if (MaterialIndexPtr == nullptr)
	{
		const int32 MaterialIndex = TrianglesByMaterial.Add(TSparseArray<FMpRenderableTriangle>());
		MaterialToIndex.Add(Material, MaterialIndex);
		return MaterialIndex;
	}

	return *MaterialIndexPtr;
}

void UMpTriangleSet::Clear()
{
	Triangles.Reset();
	TrianglesByMaterial.Reset();
	MaterialToIndex.Reset();
}

void UMpTriangleSet::ReserveTriangles(const int32 MaxID)
{
	Triangles.Reserve(MaxID);
}

int32 UMpTriangleSet::AddTriangle(const FMpRenderableTriangle& OverlayTriangle)
{
	const int32 MaterialIndex = FindOrAddMaterialIndex(OverlayTriangle.Material);
	const int32 IndexByMaterial = TrianglesByMaterial[MaterialIndex].Add(OverlayTriangle);
	const int32 ID = Triangles.Add(MakeTuple(MaterialIndex, IndexByMaterial));
	return ID;
}

void UMpTriangleSet::InsertTriangle(const int32 ID, const FMpRenderableTriangle& OverlayTriangle)
{
	const int32 MaterialIndex = FindOrAddMaterialIndex(OverlayTriangle.Material);
	const int32 IndexByMaterial = TrianglesByMaterial[MaterialIndex].Add(OverlayTriangle);
}

void UMpTriangleSet::RemoveTriangle(const int32 ID)
{
	const TTuple<int32, int32> MaterialAndTriangleIndex = Triangles[ID];
	const int32 MaterialIndex = MaterialAndTriangleIndex.Get<0>();
	const int32 IndexByMaterial = MaterialAndTriangleIndex.Get<1>();
	TSparseArray<FMpRenderableTriangle>& Container = TrianglesByMaterial[MaterialIndex];
	Container.RemoveAt(IndexByMaterial);
	if (Container.Num() == 0)
	{
		TrianglesByMaterial.RemoveAt(MaterialIndex);
	}
	Triangles.RemoveAt(ID);
}



int32 UMpTriangleSet::AddTriangle(const FVector& A, const FVector& B, const FVector& C, const FVector& Normal, const FColor& Color, const int32& Material)
{
	FMpRenderableTriangle NewTriangle;
	NewTriangle.Material = Material;

	NewTriangle.Vertex0 = { A, FVector2D(0,0), Normal, Color };
	NewTriangle.Vertex1 = { B, FVector2D(1,0), Normal, Color };
	NewTriangle.Vertex2 = { C, FVector2D(1,1), Normal, Color };

	return AddTriangle(NewTriangle);
}

FIndex2i UMpTriangleSet::AddQuad(const FVector& A, const FVector& B, const FVector& C, const FVector& D, const FVector& Normal, const FColor& Color, const int32& Material)
{
	FMpRenderableTriangle NewTriangle0;
	NewTriangle0.Material = Material;

	NewTriangle0.Vertex0 = { A, FVector2D(0,0), Normal, Color };
	NewTriangle0.Vertex1 = { B, FVector2D(1,0), Normal, Color };
	NewTriangle0.Vertex2 = { C, FVector2D(1,1), Normal, Color };

	FMpRenderableTriangle NewTriangle1 = NewTriangle0;
	NewTriangle1.Vertex1 = NewTriangle1.Vertex2;
	NewTriangle1.Vertex2 = { D, FVector2D(0,1), Normal, Color };

	int32 Index0 = AddTriangle(NewTriangle0);
	int32 Index1 = AddTriangle(NewTriangle1);
	return FIndex2i(Index0, Index1);
}


bool UMpTriangleSet::IsTriangleValid(const int32 ID) const
{
	return Triangles.IsValidIndex(ID);
}

FPrimitiveSceneProxy* UMpTriangleSetComponent::CreateSceneProxy()
{
	if (!TriangleSet)
	{
		return Super::CreateSceneProxy();
	}
	if (TriangleSet->Triangles.Num() > 0)
	{
		return new FMpTriangleSetSceneProxy(this);
	}
	return nullptr;
}

int32 UMpTriangleSetComponent::GetNumMaterials() const
{
	if (!TriangleSet)
	{
		return Super::GetNumMaterials();
	}
	return TriangleSet->TrianglesByMaterial.GetMaxIndex();
}

FBoxSphereBounds UMpTriangleSetComponent::CalcBounds(const FTransform& LocalToWorld) const
{
	if (!TriangleSet)
	{
		return Super::CalcBounds(LocalToWorld);
	}

	if (bBoundsDirty)
	{
		FBox Box(ForceInit);

		for (const TSparseArray<FMpRenderableTriangle>& TriangleArray : TriangleSet->TrianglesByMaterial)
		{
			for (const FMpRenderableTriangle& Triangle : TriangleArray)
			{
				Box += Triangle.Vertex0.Position;
				Box += Triangle.Vertex1.Position;
				Box += Triangle.Vertex2.Position;
			}
		}

		Bounds = FBoxSphereBounds(Box);
		bBoundsDirty = false;
	}

	return Bounds.TransformBy(LocalToWorld);
}