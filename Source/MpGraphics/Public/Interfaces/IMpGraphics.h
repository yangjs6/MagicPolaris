
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

//! 图形样式
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

//! 图形渲染上下文
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

//! 图形数据接收者
class IMpGraphicsDataReceiver
	: public IInterface
{
	GENERATED_BODY()
public:
	virtual void ReceiveGraphics(UMpGraphics* pGraphics) = 0;
};
