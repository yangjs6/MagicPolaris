// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

//!���֧࣬�����ü���
class IMpUnknown 
{
public:
	//!���캯��
	IMpUnknown() : mReferenceCounter(1) {}

	//!��������
	virtual ~IMpUnknown() {};

	//!������ü���
	virtual int AddRef()
	{
		return ++mReferenceCounter;
	}

	//!�������ü���
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

	//! Ĭ�ϲ�ɾ���Լ��������Ҫɾ��������д return true
	virtual bool CanDeleteSelf() { return false; }

	int mReferenceCounter;
};
