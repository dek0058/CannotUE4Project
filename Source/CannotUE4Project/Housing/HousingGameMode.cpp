
#include "HousingGameMode.h"
#include "WorldEditorBox.h"


void AHousingGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);


}

void AHousingGameMode::SelectWorldEditorBox(AActor* WorldEditorBox)
{
	CurrentWorldEditorBox = Cast<AWorldEditorBox>(WorldEditorBox);
}
