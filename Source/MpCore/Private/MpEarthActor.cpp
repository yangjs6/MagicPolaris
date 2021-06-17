#include "MpEarthActor.h"

AMpEarthActor::AMpEarthActor()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
	EarthCamera_Game = CreateDefaultSubobject<UMpEarthCamera_Game>("EarthCamera_Game");

#if WITH_EDITOR
	EarthCamera_Editor = CreateDefaultSubobject<UMpEarthCamera_Editor>("EarthCamera_Editor");
#endif
}

bool AMpEarthActor::IsGameView()
{
#if WITH_EDITOR
	return false;
#endif

	return true;
}

TScriptInterface<IMpViewCamera> AMpEarthActor::GetCamera() const
{
//#if WITH_EDITOR
//	return EarthCamera_Editor;
//#endif

	return EarthCamera_Game;
}

void AMpEarthActor::PostRegisterAllComponents()
{
	Super::PostRegisterAllComponents();
}
void AMpEarthActor::PostUnregisterAllComponents()
{
	Super::PostUnregisterAllComponents();
}

AMpEarthActor_Ellipoid::AMpEarthActor_Ellipoid()
{
	EarthModel = CreateDefaultSubobject<UMpEarthModel_Ellipoid>("EarthModel");
}

#if WITH_EDITOR

void AMpEarthActor_Ellipoid::OnConstruction(const FTransform& Transform)
{
	UpdateGeoreference();
}

void AMpEarthActor_Ellipoid::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName PropertyName = (PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None);

	// Reconstruct node to show updates to PinFriendlyNames.
	if (PropertyName == GET_MEMBER_NAME_CHECKED(AMpEarthActor_Ellipoid, OriginLongitude) ||
		PropertyName == GET_MEMBER_NAME_CHECKED(AMpEarthActor_Ellipoid, OriginLatitude))
	{
		UpdateGeoreference();
	}
}
#endif

void AMpEarthActor_Ellipoid::UpdateGeoreference()
{
	EarthModel->UpdateFrame(FGeoPosition::FromDegrees(OriginLongitude, OriginLatitude));
}

AMpEarthActor_Projection::AMpEarthActor_Projection()
{
	EarthModel = CreateDefaultSubobject<UMpEarthModel_Projection>("EarthModel");
}

void AMpEarthActor_Projection::OnConstruction(const FTransform& Transform)
{
	UpdateProjection();
}

#if WITH_EDITOR
void AMpEarthActor_Projection::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName PropertyName = (PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None);

	// Reconstruct node to show updates to PinFriendlyNames.
	if (PropertyName == GET_MEMBER_NAME_CHECKED(AMpEarthActor_Projection, SourceSRS) ||
		PropertyName == GET_MEMBER_NAME_CHECKED(AMpEarthActor_Projection, TargetSRS))
	{
		UpdateProjection();
	}
}
#endif

void AMpEarthActor_Projection::UpdateProjection()
{
	EarthModel->SetProjection(SourceSRS, TargetSRS);
}
