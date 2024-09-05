#include "MatchaPlayerController.h"
#include "MatchaCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/KismetSystemLibrary.h" 
#include "Kismet/GameplayStatics.h"
#include "MatchaGameState.h"

void AMatchaPlayerController::BeginPlay()
{
    Super::BeginPlay();

    GameState = Cast<AMatchaGameState>(UGameplayStatics::GetGameState(this));
	PlayerPawn = Cast<AMatchaCharacter>(GetPawn());
	/*if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(this->GetLocalPlayer()))
	{
		UE_LOG(LogTemp, Display, TEXT("Mapping context"));
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}*/
}
//void AMatchaPlayerController::SetupInputComponent()
//{
//	Super::SetupInputComponent();
//	// Set up action bindings
//	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent)) {
//		//Restart
//		EnhancedInputComponent->BindAction(RestartAction, ETriggerEvent::Triggered, this, &AMatchaPlayerController::RestartGame);
//	}
//}
//void AMatchaPlayerController::RestartGame(const FInputActionValue& Value)
//{
//	if (GameState && PlayerPawn) {
//		if (GameState->GetIsEndGame()) {
//			PlayerPawn->RestartLevel();
//		}
//	}
//}