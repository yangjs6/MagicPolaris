#include "MpVertexSource.h"

//////////////////////////////////////////////////////////////////////////


//! ע�����
void CMpVertexSource::RegisterEventListener(IMpVertexSourceListerner* pEventListener)
{
	EventListeners.Add(pEventListener);
}

//! �������
void CMpVertexSource::UnRegisterEventListener(IMpVertexSourceListerner* pEventListener)
{
	EventListeners.Remove(pEventListener);
}

//! ��ռ���
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

//! ��Ӷ���
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

//! ���������ӵ��Ǹ�����
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

//! �˳�����
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

//! ��õ���Χ
FGeoBoundingBox CMpVertexSource::GetBoundingBox()
{
	return BoundingBox;
}
