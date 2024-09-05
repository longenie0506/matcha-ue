// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MatchaBaseActor.h"
#include "MatchaSparkle.generated.h"

/**
 * 
 */
UCLASS()
class MATCHA_API AMatchaSparkle : public AMatchaBaseActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMatchaSparkle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool GetSparkleStatus();

	void SetSparkleStatus(bool bNewSparkleStatus);

private:
	UPROPERTY()
	bool bSparkleStatus = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UParticleSystemComponent* ParticleSystemComponent;

	void NotifyGameState();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USoundBase* SparkleSound;
};
