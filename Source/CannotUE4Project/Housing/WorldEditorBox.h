#pragma once

#include "GameFramework/Actor.h"
#include "WorldEditorBox.generated.h"

UCLASS()
class CANNOTUE4PROJECT_API AWorldEditorBox : public AActor
{
	GENERATED_BODY()
	
public:

	AWorldEditorBox();

	// AActor interface
	virtual void PreInitializeComponents() override;
	virtual void TickActor(float DeltaTime, enum ELevelTick TickType, FActorTickFunction& ThisTickFunction) override;
	virtual void Tick(float DeltaSeconds) override;
	virtual bool ShouldTickIfViewportsOnly() const override { return true; }
	// End of AActor interface

	UFUNCTION(BlueprintPure, Category = Utility)
	FVector WorldToBoxPosition(FVector LocalPosition);

	/** 해당 좌표가 WorldEditorBox내에 포함 되는지 확인합니다. */
	UFUNCTION(BlueprintPure, Category = Utility)
	bool IsInWorld(FVector WorldPosition) const;

protected:
	
	// AActor interface
	virtual void BeginPlay() override;
	// End of AActor interface

private:

	
	
public:

	static FName RootName;
	static FName WorldEditorComponentName;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component)
	TObjectPtr<class UWorldEditorComponent> WorldEditorComponent;

};
