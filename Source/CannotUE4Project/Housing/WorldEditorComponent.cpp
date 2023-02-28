#include "WorldEditorComponent.h"

#if WITH_EDITOR
#include "PrimitiveSceneProxy.h"
#include "DynamicMeshBuilder.h"
#endif

#include "WorldEditorOctree.h"
#include "Kismet/KismetSystemLibrary.h"

#if WITH_EDITOR

namespace
{

void DrawDashedLine_Internal(FPrimitiveDrawInterface* PDI, const FVector& Start, const FVector& End, const FLinearColor& Color, double DashSize, uint8 DepthPriority, float Thickness, float DepthBias)
{
	FVector LineDir = End - Start;
	double LineLeft = (End - Start).Size();
	if (LineLeft)
	{
		LineDir /= LineLeft;
	}

	const int32 nLines = FMath::CeilToInt32(LineLeft / (DashSize * 2));
	PDI->AddReserveLines(DepthPriority, nLines, DepthBias != 0);

	const FVector Dash = (DashSize * LineDir);

	FVector DrawStart = Start;
	while (LineLeft > DashSize)
	{
		const FVector DrawEnd = DrawStart + Dash;

		PDI->DrawLine(DrawStart, DrawEnd, Color, DepthPriority, Thickness, DepthBias);

		LineLeft -= 2 * DashSize;
		DrawStart = DrawEnd + Dash;
	}
	if (LineLeft > 0.0f)
	{
		const FVector DrawEnd = End;

		PDI->DrawLine(DrawStart, DrawEnd, Color, DepthPriority, Thickness, DepthBias);
	}
}

void DrawGridRectangle_Internal(FPrimitiveDrawInterface* PDI, const FMatrix& Matrix, const FVector2D& Rect, float GridSize, const FLinearColor& BorderColor, const FLinearColor& GridColor, double DashSize, uint8 DepthPriority, float Thickness, float DepthBias)
{
	const int32 NumVertices = 4;
	FVector Vertices[NumVertices] =
	{
		FVector(-Rect.X, -Rect.Y, 0.0F),
		FVector(Rect.X, -Rect.Y, 0.0F),
		FVector(-Rect.X, Rect.Y, 0.0F),
		FVector(Rect.X, Rect.Y, 0.0F),
	};

	const int32 NumIndices = 8;
	const uint32 Indices[NumIndices] =
	{
		0, 1,
		2, 3,
		0, 2,
		1, 3,
	};

	//! Draw Border
	for (int32 Index = 0; Index < NumIndices; Index += 2)
	{
		const int32 Index1 = Indices[Index];
		const int32 Index2 = Indices[Index + 1];
		const FVector& V1 = Vertices[Index1];
		const FVector& V2 = Vertices[Index2];

		PDI->DrawLine(Matrix.TransformPosition(V1), Matrix.TransformPosition(V2), BorderColor, DepthPriority, Thickness, DepthBias);
	}

	//! Draw Vertical Grid
	{
		const int32 Index1 = Indices[0];
		const int32 Index2 = Indices[1];
		const int32 Index3 = Indices[2];

		const FVector& V1 = Vertices[Index1];
		const FVector& V2 = Vertices[Index2];
		const FVector& V3 = Vertices[Index3];

		const float Dist = FVector::Distance(V1, V2);
		float GridCount = Dist / GridSize;
		const float GridStep = Dist / static_cast<int32>(GridCount);
		for (float X = V1.X; GridCount > 1.0F; --GridCount)
		{
			float Alpha = GridCount <= 0.0F ? 1.0F - GridCount : 1.0F;
			X += GridStep * Alpha;
			FVector V4(X, V1.Y, V1.Z);
			FVector V5(X, V3.Y, V1.Z);
			DrawDashedLine_Internal(PDI, Matrix.TransformPosition(V4), Matrix.TransformPosition(V5), GridColor, DashSize, DepthPriority, Thickness, DepthBias);
		}
	}

	//! Draw Horizontal Grid
	{
		const int32 Index1 = Indices[4];
		const int32 Index2 = Indices[5];
		const int32 Index3 = Indices[6];

		const FVector& V1 = Vertices[Index1];
		const FVector& V2 = Vertices[Index2];
		const FVector& V3 = Vertices[Index3];

		const float Dist = FVector::Distance(V1, V2);
		float GridCount = Dist / GridSize;
		const float GridStep = Dist / static_cast<int32>(GridCount);
		for (float Y = V1.Y; GridCount > 1.0F; --GridCount)
		{
			float Alpha = GridCount <= 0.0F ? 1.0F - GridCount : 1.0F;
			Y += GridStep * Alpha;
			FVector V4(V1.X, Y, V1.Z);
			FVector V5(V3.X, Y, V1.Z);
			DrawDashedLine_Internal(PDI, Matrix.TransformPosition(V4), Matrix.TransformPosition(V5), GridColor, DashSize, DepthPriority, Thickness, DepthBias);
		}
	}
}

}

//! FWorldEditorSceneProxy

class FWorldEditorSceneProxy : public FPrimitiveSceneProxy
{
public:

	FWorldEditorSceneProxy(const UWorldEditorComponent* InComponent)
		: FPrimitiveSceneProxy(InComponent)
		, BorderColor(InComponent->BorderColor)
		, GridColor(InComponent->GridColor)
		, Extent(InComponent->GetExtent())
		, Thickness(InComponent->Thickness)
		, DashSize(InComponent->DashSize)
		, GridSize(InComponent->GetGridSize())
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
		const FVector X = LTW.GetScaledAxis(EAxis::X);
		const FVector Y = LTW.GetScaledAxis(EAxis::Y);
		const FVector Z = LTW.GetScaledAxis(EAxis::Z);
		const FVector WorldOrigin = LTW.GetOrigin();

		const FVector HalfExtent = Extent * 0.5F;

		for (int32 ViewIndex = 0; ViewIndex < Views.Num(); ViewIndex++)
		{
			if (!(VisibilityMap & (1 << ViewIndex)))
			{
				continue;
			}

			const FSceneView* SceneView = Views[ViewIndex];
			const FVector ViewForwardVector = SceneView->GetViewDirection();

			FPrimitiveDrawInterface* PDI = Collector.GetPDI(ViewIndex);
			const FLinearColor BorderLinearColor = GetViewSelectionColor(BorderColor, *Views[ViewIndex], false, false, false, IsIndividuallySelected());
			const FLinearColor GridLinearColor = GetViewSelectionColor(GridColor, *Views[ViewIndex], false, false, false, IsIndividuallySelected());

			{	//! Draw Floor
				DrawGridRectangle_Internal(PDI, LTW, FVector2d(HalfExtent.X, HalfExtent.Y), GridSize, BorderLinearColor, GridLinearColor, DashSize, SDPG_World, Thickness, 0.0F);
			}

			{	//! Draw Celling
				const FMatrix Matrix = FMatrix(FVector(1.0F, 0.0F, 0.0F), FVector(0.0F, 1.0F, 0.0F), FVector(0.0F, 0.0F, 1.0F), FVector(0.0F, 0.0F, -Extent.Z))
					* FMatrix(X, -Y, -Z, FVector::ZeroVector)
					.ConcatTranslation(WorldOrigin);

				DrawGridRectangle_Internal
				(
					PDI,
					Matrix,
					FVector2d(HalfExtent.X, HalfExtent.Y),
					GridSize, BorderLinearColor,
					GridLinearColor,
					DashSize,
					SDPG_World,
					Thickness,
					0.0F
				);
			}

			{	//! Draw Front Wall

				const FMatrix Matrix =
					FMatrix(FVector(1.0F, 0.0F, 0.0F), FVector(0.0F, 1.0F, 0.0F), FVector(0.0F, 0.0F, 1.0F), FVector(0.0F, HalfExtent.Z, -HalfExtent.Y))
					* FMatrix(X, Z, -Y, FVector::ZeroVector)
					.ConcatTranslation(WorldOrigin);

				const float DotProduct = FVector::DotProduct(Matrix.GetColumn(2), ViewForwardVector);
				if (DotProduct > 0)
				{
					DrawGridRectangle_Internal
					(
						PDI,
						Matrix,
						FVector2d(HalfExtent.X, HalfExtent.Z),
						GridSize, BorderLinearColor,
						GridLinearColor,
						DashSize,
						SDPG_World,
						Thickness,
						0.0F
					);
				}
			}

			{	//! Draw Left Wall
				const FMatrix Matrix = FMatrix(FVector(1.0F, 0.0F, 0.0F), FVector(0.0F, 1.0F, 0.0F), FVector(0.0F, 0.0F, 1.0F), FVector(0.0F, HalfExtent.Z, -HalfExtent.Y))
					* FMatrix(FVector(1.0F, 0.0F, 0.0F), FVector(0.0F, 0.0F, 1.0F), FVector(0.0F, -1.0F, 0.0F), FVector::ZeroVector)
					* FMatrix(Y, -X, Z, FVector::ZeroVector)
					.ConcatTranslation(WorldOrigin);

				const float DotProduct = FVector::DotProduct(-Matrix.GetColumn(1), ViewForwardVector);
				if (DotProduct > 0)
				{
					DrawGridRectangle_Internal
					(
						PDI,
						Matrix,
						FVector2d(HalfExtent.Y, HalfExtent.Z),
						GridSize, BorderLinearColor,
						GridLinearColor,
						DashSize,
						SDPG_World,
						Thickness,
						0.0F
					);
				}
			}

			{	//! Draw Back Wall
				const FMatrix Matrix = FMatrix(FVector(1.0F, 0.0F, 0.0F), FVector(0.0F, 1.0F, 0.0F), FVector(0.0F, 0.0F, 1.0F), FVector(0.0F, -HalfExtent.Z, -HalfExtent.Y))
					* FMatrix(FVector(1.0F, 0.0F, 0.0F), FVector(0.0F, 0.0F, 1.0F), FVector(0.0F, -1.0F, 0.0F), FVector::ZeroVector)
					* FMatrix(FVector(1.0F, 0.0F, 0.0F), FVector(0.0F, 0.0F, 1.0F), FVector(0.0F, -1.0F, 0.0F), FVector::ZeroVector)
					* FMatrix(X, Z, -Y, FVector::ZeroVector)
					.ConcatTranslation(WorldOrigin);

				const float DotProduct = FVector::DotProduct(Matrix.GetColumn(2), ViewForwardVector);
				if (DotProduct > 0)
				{
					DrawGridRectangle_Internal
					(
						PDI,
						Matrix,
						FVector2d(HalfExtent.X, HalfExtent.Z),
						GridSize, BorderLinearColor,
						GridLinearColor,
						DashSize,
						SDPG_World,
						Thickness,
						0.0F
					);
				}
			}

			{	//! Draw Right Wall
				const FMatrix Matrix = FMatrix(FVector(1.0F, 0.0F, 0.0F), FVector(0.0F, 1.0F, 0.0F), FVector(0.0F, 0.0F, 1.0F), FVector(0.0F, HalfExtent.Z, -HalfExtent.Y))
					* FMatrix(FVector(1.0F, 0.0F, 0.0F), FVector(0.0F, 0.0F, 1.0F), FVector(0.0F, -1.0F, 0.0F), FVector::ZeroVector)
					* FMatrix(-Y, X, Z, FVector::ZeroVector)
					.ConcatTranslation(WorldOrigin);

				const float DotProduct = FVector::DotProduct(-Matrix.GetColumn(1), ViewForwardVector);
				if (DotProduct > 0)
				{
					DrawGridRectangle_Internal
					(
						PDI,
						Matrix,
						FVector2d(HalfExtent.Y, HalfExtent.Z),
						GridSize, BorderLinearColor,
						GridLinearColor,
						DashSize,
						SDPG_World,
						Thickness,
						0.0F
					);
				}
			}
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

	FColor BorderColor;
	FColor GridColor;
	FVector Extent;

	float Thickness;
	float DashSize;
	float GridSize;
};

#endif

//! UWorldEditorComponent

#if WITH_EDITOR

FPrimitiveSceneProxy* UWorldEditorComponent::CreateSceneProxy()
{
	return new FWorldEditorSceneProxy(this);
}

FBoxSphereBounds UWorldEditorComponent::CalcBounds(const FTransform& LocalToWorld) const
{
	return FBoxSphereBounds{ FBox{ FVector{-Extent.X, -Extent.Y, 0.0F}, Extent } }.TransformBy(LocalToWorld);
}

#endif

void UWorldEditorComponent::BeginPlay()
{
	Super::BeginPlay();

	FVector OctreeExtent = GetExtent();
	Octree = new FWorldEditorOctree(GetRelativeLocation() + FVector(0.0F, 0.0F, OctreeExtent.GetMax() / 2.0F), OctreeExtent.GetMax() / 2.0F);
}

void UWorldEditorComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Octree->Destroy();
	Super::EndPlay(EndPlayReason);
}


bool UWorldEditorComponent::UpdateOverlapsImpl(const TOverlapArrayView* NewPendingOverlaps, bool bDoNotifies, const TOverlapArrayView* OverlapsAtEndLocation)
{
	bool bCanSkipUpdateOverlaps = Super::UpdateOverlapsImpl(NewPendingOverlaps, bDoNotifies, OverlapsAtEndLocation);
	return bCanSkipUpdateOverlaps;
}

bool UWorldEditorComponent::OverlapComponent(const FVector& Pos, const FQuat& Rot, const FCollisionShape& CollisionShape) const
{
	bool bHit = Super::OverlapComponent(Pos, Rot, CollisionShape);


	return bHit;
}

void UWorldEditorComponent::DrawOctree()
{
	if (!Octree)
	{	//! Not initialize!
		return;
	}

	Octree->FindNodesWithPredicate(
		[](FWorldEditorOctree::FNodeIndex ParentNodeIndex, FWorldEditorOctree::FNodeIndex CurrentNodeIndex, const FBoxCenterAndExtent& NodeBounds)
		{
			return true;
		},
		[this](FWorldEditorOctree::FNodeIndex ParentNodeIndex, FWorldEditorOctree::FNodeIndex CurrentNodeIndex, const FBoxCenterAndExtent& NodeBounds)
		{
			FVector Extent = NodeBounds.Extent;
			FVector Center = NodeBounds.Center;

			DrawDebugBox(GetWorld(), Center, Extent, FColor().Blue, false, 0.0f);

			for (const FWorldEditorOctreeElement& Element : Octree->GetElementsForNode(CurrentNodeIndex))
			{
				if (nullptr == Element.Actor || !IsValid(Element.Actor))
				{
					continue;
				}
				DrawDebugBox(GetWorld(), Element.Actor->GetActorLocation(), Element.BoxCenterAndExtent.Extent, FColor().Yellow, false, 0.0f);
			}
		}
	);
}

void UWorldEditorComponent::AddElement(AActor* InActor)
{
	if (!Octree)
	{	//! Not initialize!
		return;
	}

	if (nullptr == InActor || !IsValid(InActor))
	{
		return;
	}

	FVector ActorOrigin;
	FVector ActorExtent;
	InActor->GetActorBounds(false, ActorOrigin, ActorExtent);

	if (ActorExtent.GetMax() < Octree->GetRootBounds().GetBox().GetExtent().GetMax())
	{
		FWorldEditorOctreeElement Element(FBoxCenterAndExtent(ActorOrigin, ActorExtent), InActor);
		Octree->AddElement(Element);
	}
}

FVector UWorldEditorComponent::GetExtent() const
{
	float Interval = FMath::Max(GridSize, 1.0F);
	int32 X, Y, Z;

	X = static_cast<int32>(Extent.X / Interval);
	Y = static_cast<int32>(Extent.Y / Interval);
	Z = static_cast<int32>(Extent.Z / Interval);

	return FVector(X * Interval, Y * Interval, Z * Interval);
}

FVector UWorldEditorComponent::Origin() const
{
	return GetRelativeLocation() + FVector(0.0f, 0.0F, GetExtent().Z / 2.0F);
}
