// Fill out your copyright notice in the Description page of Project Settings.


#include "MatchaCheckpoint.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AMatchaCheckpoint::AMatchaCheckpoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	ParticleSystem->SetupAttachment(RootComponent);
	ParticleSystem->Deactivate();
}

// Called when the game starts or when spawned
void AMatchaCheckpoint::BeginPlay()
{
	Super::BeginPlay();

	ParticleSystem->Deactivate();
	
}

// Called every frame
void AMatchaCheckpoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMatchaCheckpoint::ActivateEffect()
{
	ParticleSystem->Activate();
	GetWorld()->GetTimerManager().SetTimer(TimerHandle,this, &AMatchaCheckpoint::DeactivateEffect, FadeoutDuration, false);
}

void AMatchaCheckpoint::DeactivateEffect()
{
	ParticleSystem->Deactivate();
}