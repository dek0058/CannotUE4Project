#pragma once

#include "Components/PrimitiveComponent.h"
#include "WorldEditorComponent.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class CANNOTUE4PROJECT_API UWorldEditorComponent : public UPrimitiveComponent
{
	GENERATED_BODY()
	

public:

	// UPrimitiveComponent interface
	virtual FPrimitiveSceneProxy* CreateSceneProxy() override;
	virtual FBoxSphereBounds CalcBounds(const FTransform& LocalToWorld) const override;
	/*virtual FPrimitiveSceneProxy* CreateSceneProxy() override;
	virtual FBoxSphereBounds CalcBounds(const FTransform& LocalToWorld) const override;*/
	// End of UPrimitiveComponent interface


public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Draw)
	FColor BorderColor{ 0x00, 0x00, 0xFF, 0xFF };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Draw)
	FColor GridColor{ 0x00, 0x00, 0xFF, 0x80 };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Draw)
	FVector Extent{ 1000.0F, 1000.0F, 1000.0F };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Draw)
	float Thickness = 2.0F;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Draw)
	float GridSize = 100.0F;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Draw)
	float DashSize = 100.0F;

};
