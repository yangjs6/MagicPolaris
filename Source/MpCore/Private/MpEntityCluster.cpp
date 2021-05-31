#include "MpEntityCluster.h"
#include "IMpSceneView.h"
#include "KDBush.hpp"

void UMpEntityCluster::SetPixelRange(float InPixelRange)
{
	if (PixelRange != InPixelRange)
	{
		PixelRange = InPixelRange;
		MarkClusterDirty();
	}
}

void UMpEntityCluster::SetMinimumClusterSize(int InMinimumClusterSize)
{
	if (MinimumClusterSize != InMinimumClusterSize)
	{
		MinimumClusterSize = InMinimumClusterSize;
		MarkClusterDirty();
	}
}

void UMpEntityCluster::MarkClusterDirty()
{
	bClusterDirty = true;
}

UMpEntityCluster::UMpEntityCluster()
{
	ClusteredCollection = CreateDefaultSubobject<UMpEntityCollection>("ClusteredCollection");
}

struct FMpClusterPoint
{
	UMpEntity* Item;
	bool bClustered;
	FVector2D Coord;

	float getX() const { return Coord.X; }
	float getY() const { return Coord.Y; }
};

typedef std::vector<FMpClusterPoint> FMpClusterPoints;


void UMpEntityCluster::ClusterEntities(IMpSceneView* SceneView, UMpEntityCollection* EntityCollection)
{
	if (!EntityCollection || !bClusterDirty)
	{
		return;
	}
	bClusterDirty = false;

	ResetClusters();

	if (!bEnabled)
	{
		for (int i = 0; i < EntityCollection->GetNum(); ++i)
		{
			UMpEntity* Item = EntityCollection->GetEntity(i);
			Item->SetClusterShow(true);
		}
		return;
	}

	IMpViewCamera* Camera = SceneView->GetCamera();

	if (!Camera)
	{
		return;
	}

	// Get screen space points
	FMpClusterPoints Points;
	for (int i = 0; i < EntityCollection->GetNum(); ++i)
	{
		UMpEntity* Item = EntityCollection->GetEntity(i);
		FVector Postion = Item->GetPosition();
		Item->SetClusterShow(false);

		FVector2D Coord;
		//if (!Item->IsShow() || Camera->Intersets(Postion) ||
		//	!SceneView->ComputeScreenSpacePosition(Postion, Coord))
		//{
		//	continue;
		//}

		FMpClusterPoint Point;
		Point.Item = Item;
		Point.bClustered = false;
		Point.Coord = Coord;
		Points.push_back(Point);
	}

	// build kd-index for range search
	// https://github.com/mourner/kdbush
	// https://gitee.com/chenzunfeng/kdbush.hpp
	kdbush::KDBush<FMpClusterPoint, float, int> Index(Points);

	// cluster neighbor points in range of screen bounding box
	int PointNum = Points.size();
	for (int i = 0; i < PointNum; ++i)
	{
		FMpClusterPoint& Point = Points[i];
		if (Point.bClustered) 
		{
			continue;
		}

		TArray<UMpEntity*> Items;
		Items.Add(Point.Item);
		Point.bClustered = true;

		// compute screen bounding box and find neighbors using kd-index
		FBox2D BBox = Point.Item->GetScreenBoundingBox(Point.Coord, PixelRange);
		std::vector<int> Neighbors = Index.range(BBox.Min.X, BBox.Min.Y, BBox.Max.X, BBox.Max.Y);
		int NeighborNum = Neighbors.size();
		for (int j = 0; j < NeighborNum; ++j)
		{
			int NeighborIndex = Neighbors[j];
			FMpClusterPoint& NeighborPoint = Points[NeighborIndex];
			if (!NeighborPoint.bClustered)
			{
				Items.Add(NeighborPoint.Item);
			}
		}

		// add cluster items if cluster size >= MinimumClusterSize,
		// else just add non clustered item
		if (Items.Num() >= MinimumClusterSize) 
		{
			AddCluster(Items);
			for (int j = 0; j < NeighborNum; ++j)
			{
				Points[Neighbors[j]].bClustered = true;
			}
		}
		else 
		{
			AddNonClusteredItem(Point.Item);
			Point.Item->SetClusterShow(true);
		}
	}

}

void UMpEntityCluster::ResetClusters()
{
	ClusteredCollection->RemoveAll();
}

void UMpEntityCluster::AddCluster(const TArray<UMpEntity*>& Items)
{
	UMpClusterEntity* ClusterEntity = NewObject<UMpClusterEntity>(this);
	ClusterEntity->SetItems(Items);
	ClusteredCollection->Add(ClusterEntity);
}

void UMpEntityCluster::AddNonClusteredItem(UMpEntity* Item)
{
}

void UMpClusterEntity::SetItems(const TArray<UMpEntity*>& InItems)
{
	Items = InItems;
}
