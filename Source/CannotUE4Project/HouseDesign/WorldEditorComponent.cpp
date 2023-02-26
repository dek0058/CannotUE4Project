#include "WorldEditorComponent.h"

#include "PrimitiveSceneProxy.h"
#include "DynamicMeshBuilder.h"


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


/*const FVector X = Matrix.GetScaledAxis(EAxis::X);
	const FVector Y = Matrix.GetScaledAxis(EAxis::Y);
	const FVector Z = Matrix.GetScaledAxis(EAxis::Z);

	const FVector Origin = Matrix.GetOrigin();
	const FVector Extent = Extent;

	const FVector Corners[4] =
	{
		Origin + X * Extent.X + Y * Extent.Y,
		Origin + X * Extent.X - Y * Extent.Y,
		Origin - X * Extent.X - Y * Extent.Y,
		Origin - X * Extent.X + Y * Extent.Y,
	};

	for (int32 i = 0; i < 4; ++i)
	{
		PDI->DrawLine(Corners[i], Corners[(i + 1) % 4], Color, DepthPriority, Thickness, DepthBias);
	}*/
	/*
	// Get the camera and object actors
	AActor* CameraActor = // Get the camera actor
	AActor* ObjectActor = // Get the object actor

	// Get the positions of the camera and object
	FVector CameraPos = CameraActor->GetActorLocation();
	FVector ObjectPos = ObjectActor->GetActorLocation();

	// Get the forward vector of the camera
	FVector CameraForward = CameraActor->GetActorForwardVector();

	// Calculate the vector between the camera and object
	FVector CameraToObject = ObjectPos - CameraPos;

	// Normalize the vector between the camera and object
	CameraToObject.Normalize();

	// Calculate the dot product of the normalized vector between the camera and object and the forward vector of the camera
	float DotProduct = FVector::DotProduct(CameraToObject, CameraForward);

	// Check if the dot product is greater than a threshold value
	float Threshold = 0.9f; // Adjust this value as needed
	if (DotProduct > Threshold)
	{
		// Camera is looking at object
	}


	*/

}

//! FWorldEditorSceneProxy

class FWorldEditorSceneProxy : public FPrimitiveSceneProxy
{
public:

	FWorldEditorSceneProxy(const UWorldEditorComponent* InComponent)
		: FPrimitiveSceneProxy(InComponent)
		, BorderColor(InComponent->BorderColor)
		, GridColor(InComponent->GridColor)
		, Thickness(InComponent->Thickness)
		, DashSize(InComponent->DashSize)
		, GridSize(InComponent->GridSize)
	{
		bWantsSelectionOutline = false;

		float Interval = FMath::Max(GridSize, 1.0F);
		int32 X, Y, Z;

		X = static_cast<int32>(InComponent->Extent.X / Interval);
		Y = static_cast<int32>(InComponent->Extent.Y / Interval);
		Z = static_cast<int32>(InComponent->Extent.Z / Interval);

		Extent = FVector(X * Interval, Y * Interval, Z * Interval);
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

		for (int32 ViewIndex = 0; ViewIndex < Views.Num(); ViewIndex++)
		{
			if (!(VisibilityMap & (1 << ViewIndex)))
			{
				continue;
			}

			FPrimitiveDrawInterface* PDI = Collector.GetPDI(ViewIndex);
			const FLinearColor BorderLinearColor = GetViewSelectionColor(BorderColor, *Views[ViewIndex], false, false, false, IsIndividuallySelected());
			const FLinearColor GridLinearColor = GetViewSelectionColor(GridColor, *Views[ViewIndex], false, false, false, IsIndividuallySelected());

			FVector HalfExtent = Extent * 0.5F;

			const int32 NumVertices = 8;
			FVector Vertices[8] =
			{
				FVector(-HalfExtent.X, -HalfExtent.Y, 0.0F),
				FVector(HalfExtent.X, -HalfExtent.Y, 0.0F),
				FVector(-HalfExtent.X, HalfExtent.Y, 0.0F),
				FVector(HalfExtent.X, HalfExtent.Y, 0.0F),
				FVector(-HalfExtent.X, -HalfExtent.Y, Extent.Z),
				FVector(HalfExtent.X, -HalfExtent.Y, Extent.Z),
				FVector(-HalfExtent.X, HalfExtent.Y, Extent.Z),
				FVector(HalfExtent.X, HalfExtent.Y, Extent.Z)
			};

			{	//! Draw Floor
				DrawGridRectangle_Internal(PDI, LTW, FVector2d(HalfExtent.X, HalfExtent.Y), GridSize, BorderLinearColor, GridLinearColor, DashSize, SDPG_World, Thickness, 0.0F);
			}
			
			{	//! Draw Front Wall
				DrawGridRectangle_Internal
				(
					PDI,
					(
						FMatrix(FVector(1.0F, 0.0F, 0.0F), FVector(0.0F, 1.0F, 0.0F), FVector(0.0F, 0.0F, 1.0F), FVector(0.0F, HalfExtent.Y, -HalfExtent.Z))
						* FMatrix(X, Z, -Y, FVector::ZeroVector)
						)
					.ConcatTranslation(WorldOrigin),
					FVector2d(HalfExtent.X, HalfExtent.Z),
					GridSize, BorderLinearColor,
					GridLinearColor,
					DashSize,
					SDPG_World,
					Thickness,
					0.0F
				);
			}

			{	//! Draw Left Wall
				DrawGridRectangle_Internal
				(
					PDI,
					(
						FMatrix(FVector(1.0F, 0.0F, 0.0F), FVector(0.0F, 1.0F, 0.0F), FVector(0.0F, 0.0F, 1.0F), FVector(0.0F, HalfExtent.Y, -HalfExtent.Z))
						* FMatrix(FVector(1.0F, 0.0F, 0.0F), FVector(0.0F, 0.0F, 1.0F), FVector(0.0F, -1.0F, 0.0F), FVector::ZeroVector)
						* FMatrix(Y, -X, Z, FVector::ZeroVector)
						)
					.ConcatTranslation(WorldOrigin),
					FVector2d(HalfExtent.Y, HalfExtent.Z),
					GridSize, BorderLinearColor,
					GridLinearColor,
					DashSize,
					SDPG_World,
					Thickness,
					0.0F
				);
			}

			{	//! Draw Back Wall
				DrawGridRectangle_Internal
				(
					PDI,
					(
						FMatrix(FVector(1.0F, 0.0F, 0.0F), FVector(0.0F, 1.0F, 0.0F), FVector(0.0F, 0.0F, 1.0F), FVector(0.0F, -HalfExtent.Y, -HalfExtent.Z))
						* FMatrix(FVector(1.0F, 0.0F, 0.0F), FVector(0.0F, 0.0F, 1.0F), FVector(0.0F, -1.0F, 0.0F), FVector::ZeroVector)
						* FMatrix(FVector(1.0F, 0.0F, 0.0F), FVector(0.0F, 0.0F, 1.0F), FVector(0.0F, -1.0F, 0.0F), FVector::ZeroVector)
						* FMatrix(X, Z, -Y, FVector::ZeroVector)
					)
					.ConcatTranslation(WorldOrigin),
					FVector2d(HalfExtent.X, HalfExtent.Z),
					GridSize, BorderLinearColor,
					GridLinearColor,
					DashSize,
					SDPG_World,
					Thickness,
					0.0F
				);
			}

			{	//! Draw Right Wall
				DrawGridRectangle_Internal
				(
					PDI,
					(
						FMatrix(FVector(1.0F, 0.0F, 0.0F), FVector(0.0F, 1.0F, 0.0F), FVector(0.0F, 0.0F, 1.0F), FVector(0.0F, HalfExtent.Y, -HalfExtent.Z))
						* FMatrix(FVector(1.0F, 0.0F, 0.0F), FVector(0.0F, 0.0F, 1.0F), FVector(0.0F, -1.0F, 0.0F), FVector::ZeroVector)
						* FMatrix(-Y, X, Z, FVector::ZeroVector)
						)
					.ConcatTranslation(WorldOrigin),
					FVector2d(HalfExtent.Y, HalfExtent.Z),
					GridSize, BorderLinearColor,
					GridLinearColor,
					DashSize,
					SDPG_World,
					Thickness,
					0.0F
				);
			}

			{	//! Draw Celling
				DrawGridRectangle_Internal
				(
					PDI,
					(
						FMatrix(FVector(1.0F, 0.0F, 0.0F), FVector(0.0F, 1.0F, 0.0F), FVector(0.0F, 0.0F, 1.0F), FVector(0.0F, 0.0F, -Extent.Z))
						* FMatrix(X, -Y, -Z, FVector::ZeroVector)
						)
					.ConcatTranslation(WorldOrigin),
					FVector2d(HalfExtent.X, HalfExtent.Y),
					GridSize, BorderLinearColor,
					GridLinearColor,
					DashSize,
					SDPG_World,
					Thickness,
					0.0F
				);
			}

			// TODO: 벽면은 카메라가 바라보는 방향과 반대라면 보여주지 않아야 한다. (개발 필요)

			//! Calculate Camera Forward
			/*const FSceneView* SceneView = Views[ViewIndex];
			FVector CameraForwrd = LTW.TransformPosition(FVector::ZeroVector) - SceneView->ViewActor->GetActorLocation();*/

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

//! UWorldEditorComponent

FPrimitiveSceneProxy* UWorldEditorComponent::CreateSceneProxy()
{
	return new FWorldEditorSceneProxy(this);
}

FBoxSphereBounds UWorldEditorComponent::CalcBounds(const FTransform& LocalToWorld) const
{
	return FBoxSphereBounds{ FBox{ FVector{-Extent.X, -Extent.Y, 0.0F}, Extent } }.TransformBy(LocalToWorld);
}
