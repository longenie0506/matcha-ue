// Fill out your copyright notice in the Description page of Project Settings.


#include "MatchaSkillEffect.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/AudioComponent.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
//#include "NiagaraFunctionLibrary.h"
//#include "NiagaraComponent.h"

// Sets default values
AMatchaSkillEffect::AMatchaSkillEffect()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Component"));
	//EffectMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Effect Mesh"));
	SkillEffectParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Niagra Component"));

	ActivationDuration = 5.0f;
	
	RootComponent = CapsuleComponent;
	//EffectMesh->SetupAttachment(RootComponent);
	SkillEffectParticle->SetupAttachment(RootComponent);
	SkillEffectParticle->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	EffectSound = CreateDefaultSubobject<UAudioComponent>(TEXT("SoundComponent"));

}

// Called when the game starts or when spawned
void AMatchaSkillEffect::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMatchaSkillEffect::BeginDestroy()
{
	Super::BeginDestroy();
	if (EffectSound) {
		EffectSound->Stop();
	}
}

// Called every frame
void AMatchaSkillEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMatchaSkillEffect::ActivateEffect()
{
	if (SkillEffectParticle)
	{
		SkillEffectParticle->ActivateSystem();
	}
	if (EffectSound) {
		EffectSound->Play();
	}

	// Example: Use Blueprint variable
	GetWorld()->GetTimerManager().SetTimer(EffectTimerHandle, this, &AMatchaSkillEffect::DeactivateEffect, ActivationDuration, false);
	UE_LOG(LogTemp, Warning, TEXT("Activation Duration: %f"), ActivationDuration);
}

void AMatchaSkillEffect::ActivateEffectAndDestroy(float EffectDuration)
{
	if (SkillEffectParticle)
	{
		
		SkillEffectParticle->ActivateSystem();
	}
	if (EffectSound) {
		EffectSound->Play();
	}

	// Example: Use Blueprint variable
	GetWorld()->GetTimerManager().SetTimer(EffectTimerHandle, this, &AMatchaSkillEffect::DeactivateEffect, EffectDuration, false);
	
}

void AMatchaSkillEffect::DeactivateEffect()
{
	if (SkillEffectParticle)
	{
		SkillEffectParticle->DeactivateSystem();
		GetWorld()->GetTimerManager().SetTimer(EffectTimerHandle, this, &AMatchaSkillEffect::DestroyEffect, RemoveDuration, false);
	}
}

void AMatchaSkillEffect::DestroyEffect()
{
	Destroy();
}

float AMatchaSkillEffect::GetActivationDuration()
{
	return ActivationDuration;
}


