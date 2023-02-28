#include "HousingPlayerController.h"
#include "HousingDefine.h"
#include "HousingPawn.h"
#include "WorldEditorBox.h"


void AHousingPlayerController::SetWorldEditorBox(class AWorldEditorBox* WorldEditorBox)
{
	SelectedWorldEditorBox = WorldEditorBox;
}

void AHousingPlayerController::SetHousingMode(EHousingModeType Type)
{
	if (HousingModeType == Type)
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
		}
			break;
	}


	HousingModeType = Type;

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
