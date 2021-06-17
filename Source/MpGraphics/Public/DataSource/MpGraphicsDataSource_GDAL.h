#pragma once

#include "CoreMinimal.h"
#include "MpGraphicsDataSource.h"
#include "MpGraphicsDataSource_GDAL.generated.h"

UCLASS()
class MPGRAPHICS_API UMpGraphicsDataSource_GDAL :
	public UMpGraphicsDataSource
{
	GENERATED_BODY()
public:
	// Layer name
	UPROPERTY(EditAnywhere, Category = "DataSource")
		FString Name;

	// Dataset connection for shpfile(e.g. "D:/test.shp") or PostgreSQL
	// e.g. "PG:dbname=51world schema=public table=xxx/tables=xxx host=127.0.0.1 port=5432 user=postgres password=123456";
	UPROPERTY(EditAnywhere, Category = "DataSource")
		FString FileName;

	// Filter features by attribute described in the "WHERE" section of the OGR SQL
	// see also https://gdal.org/user/ogr_sql_dialect.html
	// e.g. "height > 100" or "type in('A','B')"
	UPROPERTY(EditAnywhere, Category = "DataSource")
		FString AttributeFilter;

	// Feature fields for vectorLayer 
	UPROPERTY(EditAnywhere, Category = "DataSource")
		TMap<FName, FName> FieldMap;

public:

	virtual bool Load(TScriptInterface<IMpGraphicsDataReceiver> Receiver);
};

UCLASS()
class MPGRAPHICS_API AMpGraphicsDataSourceActor_GDAL :
	public AMpGraphicsDataSourceActor
{
	GENERATED_BODY()
public:
	AMpGraphicsDataSourceActor_GDAL();

};
