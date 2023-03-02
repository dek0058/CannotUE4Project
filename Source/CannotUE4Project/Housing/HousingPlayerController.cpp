#include "HousingPlayerController.h"
#include "HousingDefine.h"
#include "HousingPawn.h"
#include "WorldEditorBox.h"


AHousingPlayerController::AHousingPlayerController()
{
}

void AHousingPlayerController::TickActor(float DeltaTime, enum ELevelTick TickType, FActorTickFunction& ThisTickFunction)
{
	Super::TickActor(DeltaTime, TickType, ThisTickFunction);

	switch (TickType)
	{
		case LEVELTICK_TimeOnly:
			break;

		case LEVELTICK_ViewportsOnly:
			break;

		case LEVELTICK_All:
		{
			OnPawnMovement();
		}
			break;

		case LEVELTICK_PauseTick:
			break;
	}
}



void AHousingPlayerController::SetWorldEditorBox(class AWorldEditorBox* WorldEditorBox)
{
	SelectedWorldEditorBox = WorldEditorBox;
}

void AHousingPlayerController::SetHousingMode(EHousingModeType Type)
{
	if (ModeType == Type)
	{
		return;
	}

	AHousingPawn* MyPawn = GetPawn<AHousingPawn>();
	if (MyPawn == nullptr && !IsValid(MyPawn))
	{
		return;
	}

	switch (Type)
	{
		case EHousingModeType::Editor:
		{
			MyPawn->SetFreedom(false);
			MyPawn->SetPosition(FVector::ZeroVector);
			MyPawn->SetActorRotation(FRotator::ZeroRotator);
		}
			break;
	}


	ModeType = Type;

}

void AHousingPlayerController::SetInputMovement(FVector2f Axis)
{
	InputData.MovementAxis.X = FMath::Clamp(Axis.X, -1.0F, 1.0F);
	InputData.MovementAxis.Y = FMath::Clamp(Axis.Y, -1.0F, 1.0F);
}

void AHousingPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	AHousingPawn* MyPawn = Cast<AHousingPawn>(aPawn);
	if (nullptr != MyPawn)
	{
		
		MyPawn->SetWorldEditorBox(SelectedWorldEditorBox);
	}

	SetHousingMode(EHousingModeType::Editor);
}

void AHousingPlayerController::OnPawnMovement()
{
	AHousingPawn* MyPawn = GetPawn<AHousingPawn>();
	if (MyPawn == nullptr && !IsValid(MyPawn))
	{
		return;
	}

	MyPawn->AddMovementAxis(InputData.MovementAxis);
}
