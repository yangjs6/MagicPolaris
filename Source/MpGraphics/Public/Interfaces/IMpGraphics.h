
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UObject/ScriptInterface.h"
#include "IMpGraphics.generated.h"

class UMaterialInterface;
class USceneComponent;

class IMpSceneView;
class IMpViewModel;

class UMpGraphics;

//////////////////////////////////////////////////////////////////////////

UINTERFACE()
class UMpGraphicsStyle : public UInterface
{
public:
	GENERATED_BODY()
};

//! ͼ����ʽ
class IMpGraphicsStyle
	: public IInterface
{
	GENERATED_BODY()
public:
	virtual UMaterialInterface* GetMaterialByName(const FName& InMaterial) = 0;
	virtual UMaterialInterface* GetMaterialById(const int& InMaterialId) = 0;
};

//////////////////////////////////////////////////////////////////////////

UINTERFACE()
class UMpGraphicsDrawContext : public UInterface
{
public:
	GENERATED_BODY()
};

//! ͼ����Ⱦ������
class IMpGraphicsDrawContext
	: public IInterface
{
	GENERATED_BODY()
public:
	virtual bool RegisterDrawComponent(USceneComponent* Component) = 0;
	virtual bool UnRegisterDrawComponent(USceneComponent* Component) = 0;

	virtual UMaterialInterface* GetDefaultMaterial() = 0;
	virtual IMpGraphicsStyle* GetStyle(const FName& InName) = 0;
};

//////////////////////////////////////////////////////////////////////////

UINTERFACE()
class UMpGraphicsDataReceiver : public UInterface
{
public:
	GENERATED_BODY()
};

//! ͼ�����ݽ�����
class IMpGraphicsDataReceiver
	: public IInterface
{
	GENERATED_BODY()
public:
	virtual void ReceiveGraphics(UMpGraphics* pGraphics) = 0;
};
