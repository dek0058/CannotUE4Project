#include "HousingPawn.h"
#include "WorldEditorBox.h"

#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/FloatingPawnMovement.h"

// Sets default values
AHousingPawn::AHousingPawn()
{
	FName RootName = TEXT("RootComponent");
	RootComponent = CreateDefaultSubobject<USceneComponent>(RootName);

	FName MovementComponentName = TEXT("HousingMovementComponent");
	HousingMovementComponent = CreateDefaultSubobject<UPawnMovementComponent, UFloatingPawnMovement>(MovementComponentName);
	HousingMovementComponent->SetUpdatedComponent(RootComponent);


	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

}

void AHousingPawn::SetWorldEditorBox(class AWorldEditorBox* WorldEditorBox)
{
	MyWorldEditorBox = WorldEditorBox;
}

void AHousingPawn::SetFreedom(bool bActive)
{

}

void AHousingPawn::SetPosition(FVector WorldPosition)
{
	if (!MyWorldEditorBox)
	{
		return;
	}

	SetActorLocation(MyWorldEditorBox->WorldToBoxPosition(WorldPosition));
}

void AHousingPawn::AddMovementAxis(FVector Axis)
{
	UPawnMovementComponent* MovementComponent = GetMovementComponent();
	if (nullptr == MovementComponent)
	{
		return;
	}

	MovementComponent->AddInputVector(Axis, true);
}
