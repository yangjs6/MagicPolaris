#include "MpVertexSource.h"

//////////////////////////////////////////////////////////////////////////


//! 注册监听
void CMpVertexSource::RegisterEventListener(IMpVertexSourceListerner* pEventListener)
{
	EventListeners.Add(pEventListener);
}

//! 解除监听
void CMpVertexSource::UnRegisterEventListener(IMpVertexSourceListerner* pEventListener)
{
	EventListeners.Remove(pEventListener);
}

//! 清空监听
void CMpVertexSource::ClearEventListeners()
{
	EventListeners.Empty();
}

void CMpVertexSource::OnPointsUpdate(IMpVertexSource* pVS, int reason)
{
	ComputeBoundingBox();
	for (int i = 0; i < EventListeners.Num(); i++)
	{
		EventListeners[i]->OnPointsUpdate(pVS, reason);
	}
}

//! 添加顶点
int CMpVertexSource::PushPoint(const FGeoPosition& Pos, int idx)
{
	if (idx < 0 || idx > Points.Num())
	{
		Points.Add(Pos);
	}
	else
	{
		Points.Insert(Pos, idx);
	}
	OnPointsUpdate(this, 0);
	return Points.Num();
}

//! 设置最后添加的那个顶点
int CMpVertexSource::ResetPoint(const FGeoPosition& Pos, int idx)
{
	if (Points.Num() <= 0)
		return 0;

	if (idx < 0 || idx > Points.Num())
	{
		idx = Points.Num() - 1;
	}
	Points[idx] = Pos;
	OnPointsUpdate(this, 1);
	return Points.Num();
}

//! 退出顶点
int CMpVertexSource::PopPoint(int idx)
{
	if (Points.Num() <= 0)
		return 0;

	if (idx < 0 || idx > Points.Num())
	{
		idx = Points.Num() - 1;
	}
	Points.RemoveAt(idx);

	OnPointsUpdate(this, 1);
	return Points.Num();
}

void CMpVertexSource::ComputeBoundingBox()
{
	if (GetPointNum() <= 0)
	{
		return;
	}
	BoundingBox.MinLocation = BoundingBox.MaxLocation = GetPoint(0);

	for (int i = 1; i < GetPointNum(); i++)
	{
		BoundingBox += GetPoint(i);
	}
}

//! 获得地理范围
FGeoBoundingBox CMpVertexSource::GetBoundingBox()
{
	return BoundingBox;
}
