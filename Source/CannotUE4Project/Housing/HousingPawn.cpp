#include "HousingPawn.h"
#include "WorldEditorBox.h"

// Sets default values
AHousingPawn::AHousingPawn()
{

	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
