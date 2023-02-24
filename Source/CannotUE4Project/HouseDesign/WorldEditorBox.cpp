#include "WorldEditorBox.h"

#include "Components/LineBatchComponent.h"

#include "WorldEditorComponent.h"


AWorldEditorBox::AWorldEditorBox()
{
	//! Create UObject
	FName RootName = TEXT("RootComponent");
	RootComponent = CreateDefaultSubobject<USceneComponent>(RootName);

	FName WorldEditorComponentName = TEXT("WorldEditorComponent");
	WorldEditorComponent = CreateDefaultSubobject<UWorldEditorComponent>(WorldEditorComponentName);
	WorldEditorComponent->SetupAttachment(RootComponent);

	//! Default Settgins
	/*PrimaryActorTick.TickGroup = TG_PrePhysics;
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.SetTickFunctionEnable(true);
	bAsyncPhysicsTickEnabled = false;*/

}

void AWorldEditorBox::PreInitializeComponents()
{
	Super::PreInitializeComponents();

}

void AWorldEditorBox::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWorldEditorBox::TickActor(float DeltaTime, enum ELevelTick TickType, FActorTickFunction& ThisTickFunction)
{
	Super::TickActor(DeltaTime, TickType, ThisTickFunction);

}
