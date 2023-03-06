#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "HousingPawn.generated.h"


UCLASS()
class CANNOTUE4PROJECT_API AHousingPawn : public APawn
{
	GENERATED_BODY()

public:
	AHousingPawn();

	UFUNCTION(BlueprintCallable)
	void SetWorldEditorBox(class AWorldEditorBox* WorldEditorBox);

	UFUNCTION(BlueprintCallable)
	void SetFreedom(bool bActive);

	UFUNCTION(BlueprintCallable)
	void SetPosition(FVector Position);

	void AddMovementAxis(FVector Axis);

public:

	static FName RootName;
	static FName UserSceneComponentName;
	static FName HousingMovementComponentName;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Field)
	TObjectPtr<class AWorldEditorBox> MyWorldEditorBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	FVector MovementDirection{ 0.0F, 0.0F, 0.0F };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	bool bFreedom = true;

private:

	/** 유저가 실제 느끼는 좌표계를 가진 컴포넌트 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Transformation, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> UserSceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPawnMovementComponent> HousingMovementComponent;

	//TObjectPtr<ArrowComp>

};
