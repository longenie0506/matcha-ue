// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BossAIController.generated.h"

/**
 * 
 */
UCLASS()
class MATCHA_API ABossAIController : public AAIController
{
	GENERATED_BODY()


protected:
    virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void StartBehaviorTree();

	UFUNCTION()
	void StopBehaviorTree();

	UFUNCTION()
	void RegisterBossTask(UBTTask_Boss* BossTask);

	UFUNCTION()
	void SetbIsActivated(bool bNewIsActivated);

	UFUNCTION()
	bool GetbIsActivated();

	void ResetAllTimers();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = true))
	class UBehaviorTree* AIBehavior;

	TArray<UBTTask_Boss*> BossTasks;

	bool bIsActivated = false;
	
	
};
