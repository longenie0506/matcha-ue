// Fill out your copyright notice in the Description page of Project Settings.


#include "MatchaLight.h"
#include "MatchaGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Components/PointLightComponent.h"


// Sets default values
AMatchaLight::AMatchaLight()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LightComponent = CreateDefaultSubobject<UPointLightComponent>(TEXT("Light Component"));
	LightComponent->SetupAttachment(RootComponent);
	LightComponent->SetIntensity(20000.f);
	LightComponent->SetVisibility(bLightStatus);
}

// Called when the game starts or when spawned
void AMatchaLight::BeginPlay()
{
	Super::BeginPlay();
	LightComponent->SetVisibility(bLightStatus);
	
}

// Called every frame
void AMatchaLight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AMatchaLight::GetLightStatus()
{
	return bLightStatus;
}

void AMatchaLight::SetLightStatus(bool bNewLightStatus)
{
	bLightStatus = bNewLightStatus;
	LightComponent->SetVisibility(bLightStatus);
	if (bLightStatus) {
		if (LightSound) {
			UGameplayStatics::SpawnSoundAtLocation(GetWorld(), LightSound, this->GetActorLocation());
		}
		NotifyGameState();
	}
}

void AMatchaLight::NotifyGameState()
{
	AMatchaGameState* GameState = Cast<AMatchaGameState>(UGameplayStatics::GetGameState(this));
	if (GameState) {
		GameState->IncrementLightTurnedOn();
	}
}

