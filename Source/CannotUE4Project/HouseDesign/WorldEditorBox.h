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

protected:
	
	// AActor interface
	virtual void BeginPlay() override;
	// End of AActor interface

private:

	
	

protected:

	UPROPERTY(VisibleAnywhere, Category = Component)
	TObjectPtr<class UWorldEditorComponent> WorldEditorComponent;

};
