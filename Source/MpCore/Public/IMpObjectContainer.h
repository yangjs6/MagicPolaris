#pragma once

#include "IMpUnknown.h"

//! 物体容器的模板实现
template<class TObjectType>
class IMpObjectContainer
{
protected:
	typedef TObjectType* TObjectPtr;
	TArray<TObjectPtr> Objects;
public:
	inline IMpObjectContainer()
	{

	}

	virtual ~IMpObjectContainer()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	//! 在遍历物体时，若存在多线程状态下，需要显示调用锁定操作
	//! 获得物体个数
	virtual int GetObjectCount() { return Objects.Num(); }

	//! 通过索引获得物体
	virtual TObjectPtr GetObjectByIdx(int idx)
	{
		int nObjects = Objects.Num();
		if (idx < 0 || idx >= nObjects)
			return nullptr;
		return Objects[idx];
	}

	//! 通过ID获得物体
	virtual TObjectPtr GetObjectById(int id) { return nullptr; }

	//////////////////////////////////////////////////////////////////////////
	// 使用以下增删改查函数时，将自动加锁，同线程内不可调用Lock否则会死锁
	//! 添加物体
	virtual void AddObject(TObjectPtr pObject)
	{
		if (!pObject)
			return;
		pObject->AddRef();
		Objects.Add(pObject);
	}

	//! 删除物体
	virtual void RemoveObject(TObjectPtr pObject)
	{
		Objects.Remove(pObject);
		pObject->Release();
	}

	//! 清空物体
	virtual void ClearObjects()
	{
		for (int i = 0; i < Objects.Num(); i++)
		{
			Objects[i]->Release();
		}
		Objects.Empty();
	}

	//! 获得物体所在索引 不是线程安全的，确保在锁内
	virtual int GetObjectIndex(TObjectPtr pObj)
	{
		int nObjects = Objects.Num();
		for (int i = 0; i < nObjects; i++)
		{
			if (Objects[i] == pObj)
				return i;
		}
		return -1;
	}

	//! 移动物体在队列中的位置，其中nStep取值在[-N,N]，N为点数，返回实际移动的步数
	virtual int MoveObject(TObjectPtr pObj, int nStep)
	{
		if (nStep == 0) return 0; // 没有做任何移动
		int nObjects = Objects.Num();
		if (nObjects <= 1) return 0; // 没有做任何移动

		int iStep = 0;
		int i = GetObjectIndex(pObj);
		if (i < 0 || i > nObjects) return 0;

		// 上移
		if (nStep > 0)
		{
			while (iStep < nStep && i > 0)
			{
				Objects[i] = Objects[i - 1];
				i--;
				iStep++;
			}
			Objects[i] = pObj;
		}
		// 下移
		else
		{
			while (iStep > nStep && i < nObjects - 1)
			{
				Objects[i] = Objects[i + 1];
				i++;
				iStep--;
			}
			Objects[i] = pObj;
		}

		return iStep;
	}

	//! 上移一层 正确应该返回1
	virtual int MoveObjectUp(TObjectPtr pObj)
	{
		return MoveObject(pObj, 1);
	}

	//! 下移一层 正确应该返回-1
	virtual int MoveObjectDown(TObjectPtr pObj)
	{
		return MoveObject(pObj, -1);
	}

	//! 上移至顶 正确应该返回正步数
	virtual int MoveObjectTop(TObjectPtr pObj)
	{
		int maxStep = Objects.Num();
		return MoveObject(pObj, maxStep);
	}

	//! 下移至底 正确应该返回负步数
	virtual int MoveObjectBottom(TObjectPtr pObj)
	{
		int maxStep = Objects.Num();
		return MoveObject(pObj, -maxStep);
	}
};

