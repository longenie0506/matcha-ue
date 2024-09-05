// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Texture2D.h"
#include "StoryStruct.generated.h"

USTRUCT(BlueprintType)
struct FStorySegmentStruct
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Story")
	UTexture2D* Image;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Story")
	FString Text;
};

USTRUCT(BlueprintType)
struct FStoryStruct {
	GENERATED_USTRUCT_BODY()

	TArray<FStorySegmentStruct> StoryStruct;
};