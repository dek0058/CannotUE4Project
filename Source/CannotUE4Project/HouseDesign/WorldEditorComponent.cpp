#include "WorldEditorComponent.h"

#include "PrimitiveSceneProxy.h"
#include "DynamicMeshBuilder.h"

//! FWorldEditorSceneProxy

class FWorldEditorSceneProxy : public FPrimitiveSceneProxy
{
public:

	FWorldEditorSceneProxy(const UWorldEditorComponent* InComponent)
		: FPrimitiveSceneProxy(InComponent)
		, BaseColor(FColor::Red)
		, Extent(InComponent->Extent)
		, LineThickness(2.0F)
		, GridSize(10.0F)
	{
		bWantsSelectionOutline = false;
	}

	virtual SIZE_T GetTypeHash() const override
	{
		static size_t UniquePointer;
		return reinterpret_cast<size_t>(&UniquePointer);
	}

	virtual void GetDynamicMeshElements(const TArray<const FSceneView*>& Views, const FSceneViewFamily& ViewFamily, uint32 VisibilityMap, FMeshElementCollector& Collector) const override
	{
		const FMatrix& LTW = GetLocalToWorld();
		const FVector WorldOrigin = LTW.GetOrigin();

		for (int32 ViewIndex = 0; ViewIndex < Views.Num(); ViewIndex++)
		{
			if (!(VisibilityMap & (1 << ViewIndex)))
			{
				continue;
			}

			FPrimitiveDrawInterface* PDI = Collector.GetPDI(ViewIndex);
			const FLinearColor Color = GetViewSelectionColor(BaseColor, *Views[ViewIndex], false, false, false, IsIndividuallySelected());

			FVector HalfExtent = Extent * 0.5F;
			//FBox BorderBox{ FVector{-Extent.X, -Extent.Y, 0.0F}, Extent };

			// Draw Border
			{
				FVector	B[2], P, Q;
				
				B[0] = FVector{ -Extent.X, -Extent.Y, 0.0F };
				B[1] = Extent;

				for (int32 i = 0; i < 2; i++)
				{
					for (int32 j = 0; j < 2; j++)
					{
						P.X = B[i].X; Q.X = B[i].X;
						P.Y = B[j].Y; Q.Y = B[j].Y;
						P.Z = B[0].Z; Q.Z = B[1].Z;
						P = LTW.TransformPosition(P);
						Q = LTW.TransformPosition(Q);
						PDI->DrawLine(P, Q, Color, SDPG_World, LineThickness, 0.0F);

						P.Y = B[i].Y; Q.Y = B[i].Y;
						P.Z = B[j].Z; Q.Z = B[j].Z;
						P.X = B[0].X; Q.X = B[1].X;
						P = LTW.TransformPosition(P);
						Q = LTW.TransformPosition(Q);
						PDI->DrawLine(P, Q, Color, SDPG_World, LineThickness, 0.0F);

						P.Z = B[i].Z; Q.Z = B[i].Z;
						P.X = B[j].X; Q.X = B[j].X;
						P.Y = B[0].Y; Q.Y = B[1].Y;
						P = LTW.TransformPosition(P);
						Q = LTW.TransformPosition(Q);
						PDI->DrawLine(P, Q, Color, SDPG_World, LineThickness, 0.0F);
					}
				}
			}

			//const FSceneView* View = Views[ViewIndex];
			/*PDI->DrawTranslucentLine(

			);*/

		}
	}

	virtual FPrimitiveViewRelevance GetViewRelevance(const FSceneView* View) const override
	{
		FPrimitiveViewRelevance Result;
		Result.bDrawRelevance = IsShown(View);
		Result.bDynamicRelevance = true;
		Result.bShadowRelevance = IsShadowCast(View);
		Result.bEditorPrimitiveRelevance = UseEditorCompositing(View);
		Result.bSeparateTranslucency = Result.bNormalTranslucency = IsShown(View);
		return Result;
	}

	virtual uint32 GetMemoryFootprint(void) const override
	{
		return sizeof(*this) + GetAllocatedSize();
	}

private:

	FColor BaseColor;
	FVector Extent;
	float LineThickness;
	float GridSize;

};

//! UWorldEditorComponent

FPrimitiveSceneProxy* UWorldEditorComponent::CreateSceneProxy()
{
	return new FWorldEditorSceneProxy(this);
}

FBoxSphereBounds UWorldEditorComponent::CalcBounds(const FTransform& LocalToWorld) const
{
	return FBoxSphereBounds{ FBox{ FVector{-Extent.X, -Extent.Y, 0.0F}, Extent } }.TransformBy(LocalToWorld);
}
