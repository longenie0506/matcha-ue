// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Boss.generated.h"

/**
 * 
 */
UCLASS()
class MATCHA_API UBTTask_Boss : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_Boss();

public:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	int32 SkillIndex = 0;

	UFUNCTION()
	void ResetTimerHandler();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

private:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	bool MultiCast = false;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	int32 MultiCastQuantity = 0;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	float MinTime = 0;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	float MaxTime = 0;
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	int32 PlayerTargetCastQuantity = 1;
	

	void CastMultipleRandomSkills();
	void CastMultipleRandomSkills(int32 index,bool bIsPlayerTarget=false);
	void ScheduleMultipleRandomSkillCasts(UBehaviorTreeComponent& OwnerComp);

	UPROPERTY()
	class UBlackboardComponent* BlackboardComp;

	FVector GetRandomLocation() const;

	class AMatchaBoss* Boss;

	FTimerHandle MultiCastTimerHandle;
	TArray<FTimerHandle> MultiCastTimerHandles;
};
