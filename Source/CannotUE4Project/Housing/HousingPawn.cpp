#include "HousingPawn.h"
#include "WorldEditorBox.h"

#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/FloatingPawnMovement.h"



FName AHousingPawn::RootName = TEXT("RootComponent");
FName AHousingPawn::UserSceneComponentName = TEXT("UserSceneComponent");
FName AHousingPawn::HousingMovementComponentName = TEXT("HousingMovementComponent");

AHousingPawn::AHousingPawn()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(RootName);

	UserSceneComponent = CreateDefaultSubobject<USceneComponent>(UserSceneComponentName);
	UserSceneComponent->SetupAttachment(RootComponent);

	HousingMovementComponent = CreateDefaultSubobject<UPawnMovementComponent, UFloatingPawnMovement>(HousingMovementComponentName);
	HousingMovementComponent->SetUpdatedComponent(RootComponent);

	//PrimaryActorTick.bCanEverTick = true;

}

void AHousingPawn::SetWorldEditorBox(class AWorldEditorBox* WorldEditorBox)
{
	MyWorldEditorBox = WorldEditorBox;
}

void AHousingPawn::SetFreedom(bool bActive)
{
	bFreedom = bActive;
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

	FVector Result{ 0.0F, 0.0F, 0.0F };

	if (Axis.Y > 0)
	{
		Result = UserSceneComponent->GetForwardVector();
	}
	else if (Axis.Y < 0)
	{
		Result = -UserSceneComponent->GetForwardVector();
	}

	if (Axis.X > 0)
	{
		Result += UserSceneComponent->GetRightVector();
	}
	else if (Axis.X < 0)
	{
		Result -= UserSceneComponent->GetRightVector();
	}

	if (MyWorldEditorBox)
	{
		if (bFreedom)
		{
			if (!MyWorldEditorBox->IsInWorld(GetActorLocation()))
			{
				// FIXME: 수정필요
				Result = FVector::ZeroVector;
			}
			else
			{
				MovementComponent->AddInputVector(Result, true);
			}
		}
		else
		{

		}
		//RootComponent->GetComponentVelocity()
		//MovementComponent->ConsumeInputVector
	}
	else
	{
		MovementComponent->AddInputVector(Result, true);
	}
}
