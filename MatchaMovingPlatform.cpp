// Fill out your copyright notice in the Description page of Project Settings.


#include "MatchaMovingPlatform.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMatchaMovingPlatform::AMatchaMovingPlatform()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMatchaMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	// Hide the actor initially
	SetActorHiddenInGame(true);

	// Disable collision initially
	SetActorEnableCollision(false);
}

// Called every frame
void AMatchaMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AMatchaMovingPlatform::GetPlatformStatus()
{
	return bPlatformStatus;
}

void AMatchaMovingPlatform::SetPlatformStatus(bool bNewPlatformStatus)
{
	bPlatformStatus = bNewPlatformStatus;
}

void AMatchaMovingPlatform::TeleportPlayer()
{
	if (ParticleTeleport) {
		UParticleSystemComponent* ParticleTeleportComponent = UGameplayStatics::SpawnEmitterAtLocation(this, ParticleTeleport,TeleportLocation,this->GetActorRotation());
		if (ParticleTeleportComponent) {
			//ParticleTeleportComponent->SetFloatParameter("EmitterLifeTime", ParticleDuration);

			FTimerHandle ParticleTimerHandle;
			GetWorld()->GetTimerManager().SetTimer(ParticleTimerHandle, [ParticleTeleportComponent]()
				{
					ParticleTeleportComponent->DestroyComponent();
				}, ParticleDuration, false);

		}
	}
}

FVector AMatchaMovingPlatform::GetTeleportLocation()
{
	return TeleportLocation;
}
