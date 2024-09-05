// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MatchaWarningZone.h"
#include "MatchaSkillEffect.h"
#include "BossSkillStruct.generated.h"

USTRUCT(BlueprintType)
struct FSkillStruct
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere,BlueprintReadWrite)
    TSubclassOf<AMatchaWarningZone> WarningZoneClass;

    UPROPERTY(EditAnywhere,BlueprintReadWrite)
    TSubclassOf<AMatchaSkillEffect> SkillEffectClass;

    //UPROPERTY(EditAnywhere,BlueprintReadWrite)
    //float WarningDuration;

    //UPROPERTY(EditAnywhere, BlueprintReadWrite)
    //float SkillDuration;
};