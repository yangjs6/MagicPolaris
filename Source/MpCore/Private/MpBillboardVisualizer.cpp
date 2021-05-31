#include "MpBillboardVisualizer.h"
#include "MpEntityCluster.h"
#include "MpBillboardEntity.h"

void UMpBillboardVisualizer::Initialize(UMpEntityCollection* InEntityCollection, UMpEntityCluster* InEntityCluster)
{
	EntityCollection = InEntityCollection;
	EntityCluster = InEntityCluster;
}

void UMpBillboardVisualizer::UpdateEntities(float Time)
{
	if (!EntityCollection)
	{
		return;
	}

	for (int i = 0; i < EntityCollection->GetNum(); i++)
	{
		UMpEntity* Entity = EntityCollection->GetEntity(i);
		UMpBillboardEntity* BillboardEntity = Cast<UMpBillboardEntity>(Entity);
		if (!BillboardEntity)
		{
			continue;
		}

		BillboardEntity->UpdateProperty(Time);
	}
}

void UMpBillboardVisualizer::UpdateView(IMpSceneView* SceneView)
{
	if (!EntityCollection || !EntityCluster)
	{
		return;
	}
	
	EntityCluster->ClusterEntities(SceneView, EntityCollection);
}
