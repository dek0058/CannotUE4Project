#pragma once

#include "GameFramework/PlayerController.h"
#include "HousingPlayerController.generated.h"


enum class EHousingModeType : uint8;

UCLASS()
class CANNOTUE4PROJECT_API AHousingPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void SetWorldEditorBox(class AWorldEditorBox* WorldEditorBox);

	UFUNCTION(BlueprintCallable)
	void SetHousingMode(EHousingModeType Type);
	
protected:

	// APlayerController interface
	virtual void OnPossess(APawn* aPawn) override;
	// End of AActor interface
	
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class AWorldEditorBox> SelectedWorldEditorBox;

	EHousingModeType HousingModeType;
};
