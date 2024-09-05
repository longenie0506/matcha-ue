// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MatchaSkillEffect.generated.h"

UCLASS()
class MATCHA_API AMatchaSkillEffect : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMatchaSkillEffect();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void ActivateEffect();

	UFUNCTION(BlueprintCallable)
	void ActivateEffectAndDestroy(float EffectDuration);

	UFUNCTION(BlueprintCallable)
	void DeactivateEffect();

	UFUNCTION(BlueprintCallable)
	void DestroyEffect();

	UFUNCTION(BlueprintPure)
	float GetActivationDuration();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = true))
	class UCapsuleComponent* CapsuleComponent;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components",meta=(AllowPrivateAccess=true))
	//UStaticMeshComponent* EffectMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UParticleSystemComponent* SkillEffectParticle; // VFX component for Niagara

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = true))
	float ActivationDuration = 5.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = true))
	float RemoveDuration = 5.0f;

	// Delegate for effect finished notification
	FTimerHandle EffectTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sounds", meta = (AllowPrivateAccess = "true"))
	UAudioComponent* EffectSound;
	
};
