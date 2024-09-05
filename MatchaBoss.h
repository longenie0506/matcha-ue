// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MatchaBaseActor.h"
#include "BossPawnBase.h"
#include "MatchaSkillEffect.h" 
#include "MatchaWarningZone.h" 
#include "BossAIController.h"
#include "BossSkillStruct.h"
#include "MatchaBoss.generated.h"

/**
 * 
 */
UCLASS()
class MATCHA_API AMatchaBoss : public ABossPawnBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMatchaBoss();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetBossStatus(bool bNewStatus);

	bool GetBossStatus();

	UFUNCTION(BlueprintCallable)
	void CastSkill();

	UFUNCTION(BlueprintCallable)
	void CastSkills(const FSkillStruct& BossSkill);

	UFUNCTION(BlueprintCallable)
	void CastSkillByIndex(int32 SkillIndex);

	UFUNCTION(BlueprintCallable)
	void CastSkillsLocation(const FSkillStruct& BossSkill,FVector CastLocation);

	UFUNCTION(BlueprintCallable)
	void CastSkillLocationByIndex(int32 SkillIndex, FVector CastLocation);

	UFUNCTION()
	void ActivateBoss();
	UFUNCTION()
	void DeactivateBoss();

	UFUNCTION()
	void ResetTimerHandle();

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BossMaskFace;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* BossParticle;

	UPROPERTY()
	bool BossStatus = false;

	void CheckPlayerInNavBounds();

	UPROPERTY(EditAnywhere, Category = "Skills", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AMatchaSkillEffect> SkillEffectBlueprint;

	UPROPERTY(EditAnywhere, Category = "Skills", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AMatchaWarningZone> WarningZoneClass;

	FTimerHandle SkillTimerHandle;

	APawn* PlayerPawn;

	// Timer handles for each skill
	TArray<FTimerHandle> SkillTimerHandles;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skills", meta = (AllowPrivateAccess = "true"))
	TArray<FSkillStruct> BossSkills;

	UPROPERTY()
	class UBehaviorTree* BehaviorTree;

	UPROPERTY()
	ABossAIController* AIController;

};
