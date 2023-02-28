#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Math/GenericOctree.h"



struct FWorldEditorOctreeElement
{
    FBoxCenterAndExtent BoxCenterAndExtent;
    TObjectPtr<AActor> Actor;

	FWorldEditorOctreeElement(FBoxCenterAndExtent InBoxCenterAndExtent, TObjectPtr<AActor> InActor)
		: BoxCenterAndExtent(InBoxCenterAndExtent)
		, Actor(InActor)
	{
	}

    bool operator==(const FWorldEditorOctreeElement& OtherElement) const
    {
        return Actor == OtherElement.Actor;
    }
};


struct FWorldEditorOctreeSematics
{
	enum { MaxElementsPerLeaf = 16 };
	enum { MinInclusiveElementsPerNode = 7 };
	enum { MaxNodeDepth = 12 };

  	typedef TInlineAllocator<MaxElementsPerLeaf> ElementAllocator;

    /**
	* Get the bounding box of the provided octree element. In this case, the box
	* is merely the point specified by the element.
	*
	* @param	Element	Octree element to get the bounding box for
	*
	* @return	Bounding box of the provided octree element
	*/
    FORCEINLINE static FBoxCenterAndExtent GetBoundingBox(const FWorldEditorOctreeElement& Element)
	{
		return Element.BoxCenterAndExtent;
	}

	/**
	* Determine if two octree elements are equal
	*
	* @param	A	First octree element to check
	* @param	B	Second octree element to check
	*
	* @return	true if both octree elements are equal, false if they are not
	*/
   	FORCEINLINE static bool AreElementsEqual(const FWorldEditorOctreeElement& A, const FWorldEditorOctreeElement& B)
    {
        return A == B;
    }

	/** Ignored for this implementation */
    FORCEINLINE static void SetElementId(const FWorldEditorOctreeElement& Element, FOctreeElementId2 Id)
	{
	}

	/**  Applies a arbitrary offset to the element */
   	FORCEINLINE static void ApplyOffset(FWorldEditorOctreeElement& Element, FVector Offset)
    {
		Element.Actor->SetActorLocation(Element.Actor->GetActorLocation() + Offset);
        Element.BoxCenterAndExtent.Center += Offset;
    }
};
