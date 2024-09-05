// Copyright Epic Games, Inc. All Rights Reserved.

#include "MatchaCharacter.h"
#include "MatchaGameState.h"
#include "MatchaLight.h" // Already inherit MatchaBaseActor.h
#include "MatchaSparkle.h" 
#include "MatchaMovingPlatform.h"
#include "MatchaBoss.h" 
#include "MatchaSkillEffect.h"
#include "MatchaCheckpoint.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"


DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AMatchaCharacter

AMatchaCharacter::AMatchaCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = JumpZVelocity;
	GetCharacterMovement()->AirControl = AirControl;
	GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
	GetCharacterMovement()->MinAnalogWalkSpeed = MinAnalogWalkSpeed;
	GetCharacterMovement()->BrakingDecelerationWalking = BrakingDecelerationWalking;
	GetCharacterMovement()->BrakingDecelerationFalling = BrakingDecelerationFalling;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AMatchaCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentHoldDuration > 0.0f && (MatchaLight!=nullptr || MatchaSparkle != nullptr || MatchaPlatform != nullptr || MatchaBoss != nullptr)) {
		CurrentHoldDuration += DeltaTime;

		if (CurrentHoldDuration > HoldDuration) {
			CurrentHoldDuration = 0.0f;
			HideInteractiveUI();

			if (MatchaLight != nullptr) {
				MatchaLight->SetLightStatus(true);
				MatchaLight = nullptr;
			}

			if (MatchaSparkle != nullptr) {
				MatchaSparkle->SetSparkleStatus(true);
				MatchaSparkle = nullptr;
			}

			if (MatchaPlatform != nullptr) {
				//MatchaPlatform->SetPlatformStatus(true); // want to reuse the platform to teleport back
				MatchaPlatform->TeleportPlayer();
				this->SetActorLocation(MatchaPlatform->GetTeleportLocation());
				this->SetActorRelativeRotation(FRotator(0.f, 0.f, -90.f));
				MatchaPlatform = nullptr;
			}

			if (MatchaBoss != nullptr) {
				MatchaBoss->SetBossStatus(true);
				MatchaBoss = nullptr;
				if (GameState && !GameState->GetBossChallengeStatus()) {
					GameState->ResetChallenge3();
				}
			}
			
		}
	}
	
}

void AMatchaCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	GameState = Cast<AMatchaGameState>(UGameplayStatics::GetGameState(this));

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	if (InteractiveWidgetClass) {
		InteractiveWidget = CreateWidget<UUserWidget>(GetWorld(), InteractiveWidgetClass);
	}

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this,&AMatchaCharacter::OnCapsuleBeginOverlap);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &AMatchaCharacter::OnCapsuleEndOverlap);

	TArray<AActor*> CheckpointArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),AMatchaCheckpoint::StaticClass(), CheckpointArray);
	if (CheckpointArray.Num() > 0) {
		CurrentCheckpoint = Cast<AMatchaCheckpoint>(CheckpointArray[0]);
	}
}





//////////////////////////////////////////////////////////////////////////
// Input
void AMatchaCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		//EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		//EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMatchaCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMatchaCharacter::Look);

		// HoldInteract
		EnhancedInputComponent->BindAction(HoldInteractAction, ETriggerEvent::Started, this, &AMatchaCharacter::StartHolding);
		EnhancedInputComponent->BindAction(HoldInteractAction, ETriggerEvent::Completed, this, &AMatchaCharacter::StopHolding);

		// RestartLevel
		EnhancedInputComponent->BindAction(RestartAction, ETriggerEvent::Triggered, this, &AMatchaCharacter::RestartLevel);

		// NextAction
		EnhancedInputComponent->BindAction(NextStoryAction, ETriggerEvent::Started, this, &AMatchaCharacter::OnNextStoryCharacter);
		
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AMatchaCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AMatchaCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AMatchaCharacter::ShowInteractiveUI()
{
	if (InteractiveWidgetClass && InteractiveWidget) {
		InteractiveWidget->AddToViewport();
	}
}

void AMatchaCharacter::HideInteractiveUI()
{
	if (InteractiveWidgetClass && InteractiveWidget) {
		InteractiveWidget->RemoveFromParent();
	}
}



void AMatchaCharacter::StartHolding()
{
	if (MatchaLight!=nullptr) {
		if (!MatchaLight->GetLightStatus()) {
			CurrentHoldDuration = 0.1f;
		}
	}

	if (MatchaSparkle != nullptr) {
		if (!MatchaSparkle->GetSparkleStatus()) {
			CurrentHoldDuration = 0.1f;
		}
	}

	if (MatchaPlatform != nullptr) {
		if (!MatchaPlatform->GetPlatformStatus()) {
			CurrentHoldDuration = 0.1f;
		}
	}

	if (MatchaBoss != nullptr) {
		if (!MatchaBoss->GetBossStatus()) {
			CurrentHoldDuration = 0.1f;
		}
	}
	
}

void AMatchaCharacter::StopHolding()
{
	CurrentHoldDuration = 0.0f;
}

float AMatchaCharacter::GetProgressBar() {
	if (HoldDuration == 0) {
		return 0.0f;
	}
	return FMath::Clamp(CurrentHoldDuration / HoldDuration, 0.0f, 1.0f);
}


void AMatchaCharacter::OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	MatchaLight = Cast<AMatchaLight>(OtherActor);
	if (MatchaLight != nullptr && !MatchaLight->GetLightStatus()) {
		ShowInteractiveUI();
	}

	MatchaSparkle = Cast<AMatchaSparkle>(OtherActor);
	if (MatchaSparkle != nullptr && !MatchaSparkle->GetSparkleStatus()) {
		ShowInteractiveUI();
	}

	MatchaPlatform = Cast<AMatchaMovingPlatform>(OtherActor);
	if (MatchaPlatform != nullptr && !MatchaPlatform->GetPlatformStatus()) {
		ShowInteractiveUI();
	}

	MatchaBoss = Cast<AMatchaBoss>(OtherActor);
	if (MatchaBoss != nullptr && !MatchaBoss->GetBossStatus()) {
		ShowInteractiveUI();
	}

	if (OtherActor && OtherActor!=this && OtherActor->IsA(AMatchaSkillEffect::StaticClass()) && CurrentCheckpoint) {
		
		if (GameState && GameState->GetBossChallengeStatus()) {
			GameState->SetBossChallengeStatus(false);
			GameState->HandleEndGame(false);
			GameState->ResetActorState();

			GetWorld()->GetTimerManager().SetTimer(SkillDeathTimer, this, &AMatchaCharacter::MovetoCheckpoint, 0.5, false);
			if (DeathEffect) {
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DeathEffect, this->GetActorTransform(), true, EPSCPoolMethod::None, true);
			}
		}
	}
	

}

void AMatchaCharacter::MovetoCheckpoint()
{
	this->SetActorLocation(CurrentCheckpoint->GetActorLocation());
	this->SetActorRelativeRotation(FRotator(0.f, 0.f, -90.f));
	CurrentCheckpoint->ActivateEffect();
}

void AMatchaCharacter::OnCapsuleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	MatchaLight = Cast<AMatchaLight>(OtherActor);
	if (MatchaLight!=nullptr) {
		HideInteractiveUI();
		MatchaLight = nullptr;
	}
	
	MatchaSparkle = Cast<AMatchaSparkle>(OtherActor);
	if (MatchaSparkle != nullptr) {
		HideInteractiveUI();
		MatchaSparkle = nullptr;
	}

	MatchaPlatform = Cast<AMatchaMovingPlatform>(OtherActor);
	if (MatchaPlatform != nullptr) {
		HideInteractiveUI();
		MatchaPlatform = nullptr;
	}

	MatchaBoss = Cast<AMatchaBoss>(OtherActor);
	if (MatchaBoss != nullptr) {
		HideInteractiveUI();
		MatchaBoss = nullptr;
	}
}

void AMatchaCharacter::RestartLevel()
{
	if (GameState) {
		if (GameState->GetIsEndGame()) {
			UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
		}
	}
	
}

void AMatchaCharacter::OnNextStoryCharacter()
{
	UE_LOG(LogTemp, Display, TEXT("On Next Story"));
	/*if (GameState && GameState->bIsInStory) {
		GameState->OnNextStory();
	}*/
}
