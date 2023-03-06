#include "WorldEditorBox.h"

#include "Components/LineBatchComponent.h"

#include "WorldEditorComponent.h"


FName AWorldEditorBox::RootName = TEXT("RootComponent");
FName AWorldEditorBox::WorldEditorComponentName = TEXT("WorldEditorComponent");

AWorldEditorBox::AWorldEditorBox()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(RootName);

	WorldEditorComponent = CreateDefaultSubobject<UWorldEditorComponent>(WorldEditorComponentName);
	WorldEditorComponent->SetupAttachment(RootComponent);

	//! Default Settgins
	PrimaryActorTick.TickGroup = TG_PrePhysics;
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.SetTickFunctionEnable(true);
	bAsyncPhysicsTickEnabled = false;

	bNetLoadOnClient = false;

	// will be updated in PostInitProperties
	SetCanBeDamaged(false);
}

void AWorldEditorBox::PreInitializeComponents()
{
	Super::PreInitializeComponents();

}

void AWorldEditorBox::TickActor(float DeltaTime, enum ELevelTick TickType, FActorTickFunction& ThisTickFunction)
{
	Super::TickActor(DeltaTime, TickType, ThisTickFunction);

	//FColor BoxColor = FColor::Red;
	//DrawDebugBox(GetWorld(), GetActorLocation() + FVector(0.0F, 0.0F, 25.0F), FVector(500.0F, 500.0F, 25.0F), FQuat::Identity, BoxColor, true, -1.0f, 0, 2.0f);

}

void AWorldEditorBox::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (WorldEditorComponent)
	{
		WorldEditorComponent->DrawOctree();
	}
}

FVector AWorldEditorBox::WorldToBoxPosition(FVector WorldPosition)
{
	return GetActorLocation();
}

bool AWorldEditorBox::IsInWorld(FVector WorldPosition) const
{
	return WorldEditorComponent->IsInWorld(WorldPosition);
}

void AWorldEditorBox::BeginPlay()
{
	Super::BeginPlay();

}

