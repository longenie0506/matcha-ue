// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MatchaBaseActor.h"
#include "MatchaMovingPlatform.generated.h"

/**
 * 
 */
UCLASS()
class MATCHA_API AMatchaMovingPlatform : public AMatchaBaseActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AMatchaMovingPlatform();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool GetPlatformStatus();

	void SetPlatformStatus(bool bNewPlatformStatus);

	void TeleportPlayer();

	FVector GetTeleportLocation();

private:
	UPROPERTY()
	bool bPlatformStatus = false;

	UPROPERTY(EditAnywhere, Category = "Teleport")
	FVector TeleportLocation = this->GetActorLocation();

	UPROPERTY(EditAnywhere,Category = "Teleport")
	float ParticleDuration = 3.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Teleport",meta=(AllowPrivateAccess=true))
	class UParticleSystem* ParticleTeleport;




};
