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

#if WITH_EDITOR
	// UPrimitiveComponent interface
	virtual FPrimitiveSceneProxy* CreateSceneProxy() override;
	virtual FBoxSphereBounds CalcBounds(const FTransform& LocalToWorld) const override;
#endif
	virtual bool UpdateOverlapsImpl(const TOverlapArrayView* NewPendingOverlaps = nullptr, bool bDoNotifies = true, const TOverlapArrayView* OverlapsAtEndLocation = nullptr) override;
	virtual bool OverlapComponent(const FVector& Pos, const FQuat& Rot, const FCollisionShape& CollisionShape) const override;
	// End of UPrimitiveComponent interface

	UFUNCTION(BlueprintGetter)
	FVector GetExtent() const;

	UFUNCTION(BlueprintGetter)
	float GetGridSize() const
	{
		return GridSize;
	}

public:

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Draw)
	FColor BorderColor{ 0xFF, 0xFF, 0xFF, 0xFF };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Draw)
	FColor GridColor{ 0xFF, 0xFF, 0xFF, 0x80 };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Draw)
	float Thickness = 2.0F;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Draw)
	float DashSize = 10;
#endif

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Draw, BlueprintGetter = GetGridSize)
	float GridSize = 100.0F;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Draw, BlueprintGetter = GetExtent)
	FVector Extent{ 1000.0F, 1000.0F, 1000.0F };

	//TOctree2<TObjectPtr<AActor>> OctZone;
};
