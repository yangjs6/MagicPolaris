#pragma once

#include "IMpUnknown.h"
#include "MpTypes.h"

class IMpVertexSource;

//! 顶点源变更事件监听
class IMpVertexSourceListerner : public IMpUnknown
{
public:
	virtual void OnPointsUpdate(IMpVertexSource* pVS, int reason = 0) = 0;
};

//! 顶点数据源，只有一系列离散的点
//!  不具有线的概念，如需要线的概念需要自行取值并构造转换
class IMpVertexSource : public IMpUnknown
{
public:
	//! 注册监听
	virtual void RegisterEventListener(IMpVertexSourceListerner* pEventListener) = 0;

	//! 解除监听
	virtual void UnRegisterEventListener(IMpVertexSourceListerner* pEventListener) = 0;

	//! 获得点数
	virtual int GetPointNum() = 0;

	//! 获得第idx个点
	virtual const FGeoPosition& GetPoint(int idx) = 0;

	//! 预先设置内存顶点数量
	virtual void ReservePointNum(int num) = 0;

	//! 添加顶点,idx为插入的点位置，小于0时为最后一个点位置
	virtual int PushPoint(const FGeoPosition& Pos, int idx = -1) = 0;

	//! 设置最后添加的那个顶点,idx为设置的点位置，小于0时为最后一个点位置
	virtual int ResetPoint(const FGeoPosition& Pos, int idx = -1) = 0;

	//! 退出顶点
	virtual int PopPoint(int idx = -1) = 0;

	//! 清空顶点
	virtual void ClearPoints() = 0;

	//! 获得地理范围
	virtual FGeoBoundingBox GetBoundingBox() = 0;
};
