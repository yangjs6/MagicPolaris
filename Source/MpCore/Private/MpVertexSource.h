#pragma once

#include "IMpVertexSource.h"

//! 数据顶点源
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

	//! 注册监听
	virtual void RegisterEventListener(IMpVertexSourceListerner* pEventListener);

	//! 解除监听
	virtual void UnRegisterEventListener(IMpVertexSourceListerner* pEventListener);

	//! 获得点数
	virtual int GetPointNum() { return Points.Num(); }

	//! 获得第idx个点
	virtual const FGeoPosition& GetPoint(int idx) { return Points[idx]; }

	//! 预先设置内存顶点数量
	virtual void ReservePointNum(int num) { Points.Reserve(num); }

	//! 添加顶点
	virtual int PushPoint(const FGeoPosition& Pos, int idx = -1);

	//! 设置最后添加的那个顶点
	virtual int ResetPoint(const FGeoPosition& Pos, int idx = -1);

	//! 退出顶点
	virtual int PopPoint(int idx = -1);

	//! 清空顶点
	virtual void ClearPoints() { Points.Empty(); }

	//! 获得地理范围
	virtual FGeoBoundingBox GetBoundingBox();

protected:
	virtual void OnPointsUpdate(IMpVertexSource* pVS, int reason = 0);

	//! 清空监听
	void ClearEventListeners();

	//! 计算地理范围
	void ComputeBoundingBox();
};
