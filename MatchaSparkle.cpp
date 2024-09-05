// Fill out your copyright notice in the Description page of Project Settings.


#include "MatchaSparkle.h"
#include "Particles/ParticleSystemComponent.h"
#include "MatchaGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Distributions/DistributionFloatConstant.h"

// Sets default values
AMatchaSparkle::AMatchaSparkle()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>("Particle Effect");
	ParticleSystemComponent->SetupAttachment(RootComponent);
	
}

// Called when the game starts or when spawned
void AMatchaSparkle::BeginPlay()
{
	Super::BeginPlay();

	if (ParticleSystemComponent) {
		ParticleSystemComponent->DeactivateSystem();
	}
	// Hide the actor initially
	SetActorHiddenInGame(true);

	// Disable collision initially
	SetActorEnableCollision(false);

}

// Called every frame
void AMatchaSparkle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AMatchaSparkle::GetSparkleStatus()
{
	return bSparkleStatus;
}

void AMatchaSparkle::SetSparkleStatus(bool bNewSparkleStatus)
{
	bSparkleStatus = bNewSparkleStatus;
	if (bSparkleStatus==true) {
        if (ParticleSystemComponent)
        {
			if (SparkleSound) {
				UGameplayStatics::SpawnSoundAtLocation(GetWorld(), SparkleSound, this->GetActorLocation());
			}
			
			ParticleSystemComponent->DeactivateSystem();
			NotifyGameState();
        }
	}
	else {
		if (ParticleSystemComponent)
		{
			ParticleSystemComponent->ActivateSystem();
		}
	}
}

void AMatchaSparkle::NotifyGameState()
{
	AMatchaGameState* GameState = Cast<AMatchaGameState>(UGameplayStatics::GetGameState(this));
	if (GameState) {
		GameState->IncrementSparkleCollected();
	}
}
