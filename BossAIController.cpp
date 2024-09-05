// Fill out your copyright notice in the Description page of Project Settings.


#include "BossAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BTTask_Boss.h"
#include "MatchaBoss.h" // Make sure this is the correct header for your boss actor
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"


void ABossAIController::BeginPlay()
{
    Super::BeginPlay();

	StartBehaviorTree();
}




// Called every frame
void ABossAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ABossAIController::StartBehaviorTree()
{
	if (AIBehavior != nullptr) {
		RunBehaviorTree(AIBehavior);
	}
}

void ABossAIController::StopBehaviorTree()
{
	if (AIBehavior != nullptr) {
		//RunBehaviorTree(AIBehavior);
		
		this->BrainComponent->StopLogic(TEXT("Stop AI Behavior"));
	}
}

void ABossAIController::RegisterBossTask(UBTTask_Boss* BossTask)
{
	if (BossTask != nullptr)
	{
		BossTasks.Add(BossTask);
	}
}

void ABossAIController::SetbIsActivated(bool bNewIsActivated)
{
	bIsActivated = bNewIsActivated;
}

bool ABossAIController::GetbIsActivated()
{
	return bIsActivated;
}

void ABossAIController::ResetAllTimers()
{
	UBehaviorTreeComponent* BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BehaviorTreeComponent)
	{
		for (UBTTask_Boss* TaskNode : BossTasks)
		{
			TaskNode->ResetTimerHandler();
		}
	}
}

