// Fill out your copyright notice in the Description page of Project Settings.


#include "MatchaWarningZone.h"
#include "Particles/ParticleSystemComponent.h"
#include "TimerManager.h"

// Sets default values
AMatchaWarningZone::AMatchaWarningZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	WarningZoneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WarningZoneMesh"));
	WarningZoneParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("WarningZoneParticle"));
	//RootComponent = WarningZoneMesh;


	WarningZoneMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WarningZoneMesh->SetVisibility(false);
	WarningZoneMesh->SetupAttachment(RootComponent);

	WarningZoneParticle->SetupAttachment(RootComponent);

	if (WarningZoneParticle) {
		WarningZoneParticle->DeactivateSystem();
	}
	

	//WarningArea = CreateDefaultSubobject<UBoxComponent>(TEXT("WarningArea"));
	//RootComponent = WarningArea;
	//WarningArea->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//WarningArea->SetVisibility(false);

}

// Called when the game starts or when spawned
void AMatchaWarningZone::BeginPlay()
{
	Super::BeginPlay();

	if (WarningZoneMaterial)
	{
		WarningZoneMesh->SetMaterial(0, WarningZoneMaterial);
	}
}

// Called every frame
void AMatchaWarningZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMatchaWarningZone::ShowWarning()
{
	WarningZoneMesh->SetVisibility(true);
	if (WarningZoneParticle) {
		WarningZoneParticle->ActivateSystem();
	}
}

void AMatchaWarningZone::ShowWarningAndDestroy()
{
	WarningZoneMesh->SetVisibility(true);
	if (WarningZoneParticle) {
		WarningZoneParticle->ActivateSystem();
	}
	GetWorld()->GetTimerManager().SetTimer(EffectTimerHandle, this, &AMatchaWarningZone::HideWarning, WarningDuration, false);
}

void AMatchaWarningZone::HideWarning()
{
	WarningZoneMesh->SetVisibility(false);
	if (WarningZoneParticle) {
		WarningZoneParticle->DeactivateSystem();
	}
	GetWorld()->GetTimerManager().SetTimer(EffectTimerHandle, this, &AMatchaWarningZone::DestroyWarning, WarningDuration, false);
}
void AMatchaWarningZone::DestroyWarning()
{
	Destroy();
}

float AMatchaWarningZone::GetWarningDuration()
{
	return WarningDuration;
}


