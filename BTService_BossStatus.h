// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_BossStatus.generated.h"

/**
 * 
 */
UCLASS()
class MATCHA_API UBTService_BossStatus : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_BossStatus();

public:
	// Called every frame
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;


};
