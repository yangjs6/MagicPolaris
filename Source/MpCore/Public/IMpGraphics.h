#pragma once

#include "IMpUnknown.h"
#include "IMpVertexSource.h"

class IMpSchema;
class IMpVertexSource;
class IMpSceneView;

//!图形
class IMpGraphics : public IMpUnknown
{
public:
	//! 获得图形符号
	virtual IMpSchema* GetSchema() const = 0;

	//!挂载数据源
	virtual void SetVertexSource(IMpVertexSource* pVertexSource) = 0;

	//!获得数据源
	virtual IMpVertexSource* GetVertexSource() const = 0;
	//////////////////////////////////////////////////////////////////////////
	//!重构图形
	virtual void RebuildGraphics() = 0;

	//!更新图形
	virtual void UpdateGraphics(IMpSceneView* pView, float DeltaTime) = 0;

	//! 获得属性值
	virtual bool GetPropertyValue(const FName& PropertyName, FString& PropertyValue) const = 0;

	//! 设置属性值
	virtual bool SetPropertyValue(const FName& PropertyName, const FString& PropertyValue) = 0;

};

//!图形构建器插件
class IMpGraphicsCreator : public IMpUnknown
{
public:
	//! 预判断过滤条件
	virtual bool CanCreateGraphics(IMpSchema* pSchema) const = 0;

	//! 创建图形
	virtual IMpGraphics* CreateGraphics(IMpSchema* pSchema) = 0;
};

//!图形工厂
class IMpGraphicsFactory : public IMpUnknown
{
public:
	//! 注册图形插件
	virtual void RigisterGraphicsCreator(IMpGraphicsCreator* pGraphicsCreator) = 0;

	//! 取消注册图形插件
	virtual void UnRigisterGraphicsCreator(IMpGraphicsCreator* pGraphicsCreator) = 0;

	//! 获得创造器
	virtual IMpGraphicsCreator* GetGraphicsCreateor(IMpSchema* pSchema) const = 0;

	//! 创建图形
	virtual IMpGraphics* CreateGraphics(IMpSchema* pSchema) = 0;

	//! 预判断过滤条件
	virtual bool CanCreateGraphics(IMpSchema* pSchema) const = 0;
};
