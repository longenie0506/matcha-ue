// Copyright Epic Games, Inc. All Rights Reserved.

#include "MatchaGameMode.h"
#include "MatchaCharacter.h"
#include "MatchaGameState.h"
#include "MatchaLight.h"
#include "MatchaPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"


AMatchaGameMode::AMatchaGameMode()
{
	PlayerControllerClass = AMatchaPlayerController::StaticClass();
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void AMatchaGameMode::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> FoundLights;
	UGameplayStatics::GetAllActorsOfClass(this, AMatchaLight::StaticClass(), FoundLights);

	UE_LOG(LogTemp, Display, TEXT("%d"), FoundLights.Num());

	AMatchaGameState* MatchaGameState = Cast<AMatchaGameState>(UGameplayStatics::GetGameState(this));
	if (GameState) {
		MatchaGameState->SetTotalLight(FoundLights.Num());
	}
}