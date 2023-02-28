// Fill out your copyright notice in the Description page of Project Settings.

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

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class AWorldEditorBox> MyWorldEditorBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input)
	FVector InputMovementDirection{ 0.0F, 0.0F, 0.0F };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Housing)
	bool bFreedom = true;

};
