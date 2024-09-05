// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "StoryStruct.h"
#include "StoryObject.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class MATCHA_API UStoryObject : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Story")
	FStoryStruct FullStory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Story")
	TArray<FStorySegmentStruct> FullStorySegment;
};
