#pragma once

#include "IMpUnknown.h"
#include "IMpObjectContainer.h"

enum EMpSchemaType
{
	SchemaType_Null = 0,
	SchemaType_Boolean,
	SchemaType_Integer,
	SchemaType_Number,
	SchemaType_String,
	SchemaType_Object,
	SchemaType_Array,
	SchemaType_Count
};

class FMpSchemaEnumValue
{
public:
	FName Name;
	FString Description;
};

class IMpSchema : public IMpUnknown
{
public:
	//! 获得名称
	virtual const FName& GetName() = 0;

	//! 获得描述
	virtual const FString& GetDescription() = 0;

	//! 是否是值类型
	virtual bool IsValue() = 0;

	////! 获得扩展
	//virtual const FString& GetExtensionPrefix() = 0;

	//! 获得值类型
	virtual TArray<EMpSchemaType> GetTypes() = 0;

	//! 获得扩展
	virtual TArray<IMpSchema*> GetExtends() = 0;

	//! 获得属性列表
	virtual TArray<IMpProperty*> GetProperties() = 0;

	//! 获得增量属性，默认返回nullptr
	virtual IMpProperty* GetAdditionalProperties() = 0;

	//! 获得枚举值
	virtual TArray<FMpSchemaEnumValue> GetEnumValues() = 0;
};

class IMpProperty : public IMpUnknown
{
public:
	//! 获得名称
	virtual const FName& GetName() = 0;

	//! 获得描述
	virtual const FString& GetDescription() = 0;

	//! 获得值类型
	virtual IMpSchema* GetValueType() = 0;

	//! 获得默认值
	virtual FString GetDefaultValue() = 0;
};

class IMpSchemaManager : public IMpUnknown,
	public IMpObjectContainer<IMpSchema>
{
public:
	virtual IMpSchema* GetSchemaByName() = 0;
};