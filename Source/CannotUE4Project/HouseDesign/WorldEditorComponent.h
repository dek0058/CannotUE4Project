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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Shape)
	FVector Extent{ 100.0F, 100.0F, 100.0F };

};
