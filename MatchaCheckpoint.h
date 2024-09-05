// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MatchaCheckpoint.generated.h"

UCLASS()
class MATCHA_API AMatchaCheckpoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMatchaCheckpoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ActivateEffect();

	void DeactivateEffect();
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UParticleSystemComponent* ParticleSystem;

	UPROPERTY(EditAnywhere, Category = "Teleport")
	float FadeoutDuration = 3.0f;

	FTimerHandle TimerHandle;
};
