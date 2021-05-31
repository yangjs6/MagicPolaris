// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

//!基类，支持引用计数
class IMpUnknown 
{
public:
	//!构造函数
	IMpUnknown() : mReferenceCounter(1) {}

	//!析构函数
	virtual ~IMpUnknown() {};

	//!添加引用计数
	virtual int AddRef()
	{
		return ++mReferenceCounter;
	}

	//!减少引用计数
	virtual int Release()
	{
		mReferenceCounter--;
		if (!mReferenceCounter) 
		{
			if (CanDeleteSelf())
			{
				delete this;
			}
			return 0;
		}
		else 
		{
			return mReferenceCounter;
		}
	}

protected:

	//! 默认不删除自己，如果需要删除，请重写 return true
	virtual bool CanDeleteSelf() { return false; }

	int mReferenceCounter;
};
