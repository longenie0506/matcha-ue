// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_BossStatus.h"
#include "MatchaBoss.h"
#include "BossAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"
#include "AIController.h"

UBTService_BossStatus::UBTService_BossStatus()
{
	NodeName = "Update Status Boss";
}

void UBTService_BossStatus::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    AAIController* AIController = OwnerComp.GetAIOwner();
    if (AIController)
    {
        ABossAIController* BossAIController = Cast<ABossAIController>(AIController);
        if (BossAIController)
        {
            AMatchaBoss* MatchaBoss = Cast<AMatchaBoss>(BossAIController->GetPawn());
            if (MatchaBoss)
            {
                OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("isActivated"), MatchaBoss->GetBossStatus());
            }
        }
    }
}
