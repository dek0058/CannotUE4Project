#pragma once

#include "GameFramework/PlayerController.h"
#include "HousingPlayerController.generated.h"


enum class EHousingModeType : uint8;


USTRUCT()
struct FHousingInputData
{
	GENERATED_USTRUCT_BODY()

public:
	FVector MovementAxis;

	FHousingInputData()
		: MovementAxis(FVector::ZeroVector)
	{
	}
};


UCLASS()
class CANNOTUE4PROJECT_API AHousingPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	AHousingPlayerController();

	//~ Begin AActor Interface
	virtual void TickActor(float DeltaTime, enum ELevelTick TickType, FActorTickFunction& ThisTickFunction) override;
	//~ End AActor Interface

	UFUNCTION(BlueprintCallable)
	void SetWorldEditorBox(class AWorldEditorBox* WorldEditorBox);

	UFUNCTION(BlueprintCallable)
	void SetHousingMode(EHousingModeType Type);
	
	UFUNCTION(BlueprintCallable, Category = Input)
	void SetInputMovement(FVector2f Axis);

protected:

	// APlayerController interface
	virtual void OnPossess(APawn* aPawn) override;
	// End of AActor interface

	/** �Է� ���� ������ ���� �����Դϴ�. */
	void OnPawnMovement();
	
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class AWorldEditorBox> SelectedWorldEditorBox;

	EHousingModeType ModeType;

	FHousingInputData InputData;
};
