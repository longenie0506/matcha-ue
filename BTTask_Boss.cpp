// Fill out your copyright notice in the Description page of Project Settings.



#include "BTTask_Boss.h"
#include "AIController.h"
#include "BossAIController.h"
#include "MatchaBoss.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "TimerManager.h"

UBTTask_Boss::UBTTask_Boss() {
	NodeName = "Casting skills";
}

void UBTTask_Boss::ResetTimerHandler()
{
	
	for (FTimerHandle& TimerHandle : MultiCastTimerHandles)
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}
	MultiCastTimerHandles.Empty();
	
	GetWorld()->GetTimerManager().ClearTimer(MultiCastTimerHandle);
}

EBTNodeResult::Type UBTTask_Boss::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	Super::ExecuteTask(OwnerComp, NodeMemory);

	/*if (OwnerComp.GetAIOwner() == nullptr) {
		return EBTNodeResult::Failed;
	}*/

	if (ABossAIController* AIController = Cast<ABossAIController>(OwnerComp.GetAIOwner()))
	{
		AIController->RegisterBossTask(this);
	}

	Boss = Cast<AMatchaBoss>(OwnerComp.GetAIOwner()->GetPawn());
	BlackboardComp = OwnerComp.GetBlackboardComponent();

	if (Boss == nullptr || BlackboardComp == nullptr) {
		
		if (Boss == nullptr) {
			UE_LOG(LogTemp, Display, TEXT("no boss"));
		}
		if (BlackboardComp == nullptr) {
			UE_LOG(LogTemp, Display, TEXT("no blackboard"));
		}
		return EBTNodeResult::Failed;
	}

	if (BlackboardComp->GetValueAsBool("IsActivated")) {
		ScheduleMultipleRandomSkillCasts(OwnerComp);
	}

	return EBTNodeResult::Succeeded;
}

void UBTTask_Boss::CastMultipleRandomSkills()
{
	if (BlackboardComp->GetValueAsBool("IsActivated")) {
		FVector TargetActorLocation = BlackboardComp->GetValueAsVector("PlayerLocation");
		FVector RandomLocation = GetRandomLocation();
		if (RandomLocation != FVector::ZeroVector && TargetActorLocation != FVector::ZeroVector)
		{
			FRotator RandomRotation = FRotator::ZeroRotator;
			Boss->CastSkillLocationByIndex(SkillIndex, RandomLocation);
		}
	}
}

void UBTTask_Boss::CastMultipleRandomSkills(int32 index, bool bIsPlayerTarget)
{
	
	if (BlackboardComp->GetValueAsBool("IsActivated")) {
		FVector RandomLocation = FVector::ZeroVector;
		if (bIsPlayerTarget) {
			FVector TargetActorLocation = BlackboardComp->GetValueAsVector("PlayerLocation");
			RandomLocation = TargetActorLocation;
		}
		else {
			RandomLocation = GetRandomLocation();
		}

		if (RandomLocation != FVector::ZeroVector)
		{
			FRotator RandomRotation = FRotator::ZeroRotator;
			Boss->CastSkillLocationByIndex(SkillIndex, RandomLocation);
		}
		if (MultiCastTimerHandles.IsValidIndex(index))
		{
			MultiCastTimerHandles.RemoveAt(index);
		}
	}
}

void UBTTask_Boss::ScheduleMultipleRandomSkillCasts(UBehaviorTreeComponent& OwnerComp)
{
	if (MultiCast && MultiCastQuantity > 0) {
		for (int32 i = 0; i < MultiCastQuantity; ++i)
		{
			float RandomDelay = FMath::RandRange(MinTime, MaxTime);
			
			FTimerDelegate TimerDel = FTimerDelegate::CreateUObject(this, &UBTTask_Boss::CastMultipleRandomSkills, i,false);
			FTimerHandle TimerHandle;

			OwnerComp.GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, RandomDelay, false);
			MultiCastTimerHandles.Add(TimerHandle);
			
		}
		for (int32 i = 0; i < PlayerTargetCastQuantity; ++i)
		{
			float RandomDelay = FMath::RandRange(MinTime, MaxTime);

			FTimerDelegate TimerDel = FTimerDelegate::CreateUObject(this, &UBTTask_Boss::CastMultipleRandomSkills, i, true);
			FTimerHandle TimerHandle;

			OwnerComp.GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, RandomDelay, false);
			MultiCastTimerHandles.Add(TimerHandle);

		}
	}
	else {
		CastMultipleRandomSkills();
	}
}


FVector UBTTask_Boss::GetRandomLocation() const
{
	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	if (NavSys)
	{
		FVector Origin = Boss ? Boss->GetActorLocation() : FVector::ZeroVector;
		float Radius = 3000.0f; 

		FNavLocation RandomLocation;
		bool bFound = NavSys->GetRandomPointInNavigableRadius(Origin, Radius, RandomLocation);
		
		if (bFound)
		{
			return RandomLocation.Location;
		}
	}

	return FVector::ZeroVector;
}