#pragma once

#include "IMpVertexSource.h"

//! ���ݶ���Դ
class CMpVertexSource : public IMpVertexSource, public IMpVertexSourceListerner
{
	TArray<FGeoPosition> Points;
	TArray<IMpVertexSourceListerner*> EventListeners;

	FGeoBoundingBox BoundingBox;
public:
	~CMpVertexSource()
	{
		ClearPoints();
		ClearEventListeners();
	}

	//! ע�����
	virtual void RegisterEventListener(IMpVertexSourceListerner* pEventListener);

	//! �������
	virtual void UnRegisterEventListener(IMpVertexSourceListerner* pEventListener);

	//! ��õ���
	virtual int GetPointNum() { return Points.Num(); }

	//! ��õ�idx����
	virtual const FGeoPosition& GetPoint(int idx) { return Points[idx]; }

	//! Ԥ�������ڴ涥������
	virtual void ReservePointNum(int num) { Points.Reserve(num); }

	//! ��Ӷ���
	virtual int PushPoint(const FGeoPosition& Pos, int idx = -1);

	//! ���������ӵ��Ǹ�����
	virtual int ResetPoint(const FGeoPosition& Pos, int idx = -1);

	//! �˳�����
	virtual int PopPoint(int idx = -1);

	//! ��ն���
	virtual void ClearPoints() { Points.Empty(); }

	//! ��õ���Χ
	virtual FGeoBoundingBox GetBoundingBox();

protected:
	virtual void OnPointsUpdate(IMpVertexSource* pVS, int reason = 0);

	//! ��ռ���
	void ClearEventListeners();

	//! �������Χ
	void ComputeBoundingBox();
};
