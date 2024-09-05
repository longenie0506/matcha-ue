// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "MatchaCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AMatchaCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* HoldInteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* RestartAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* NextStoryAction;

public:
	AMatchaCharacter();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	float GetProgressBar();

	UFUNCTION(BlueprintCallable)
	void RestartLevel();

	UFUNCTION()
	void OnNextStoryCharacter();
	

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

protected:
	void ShowInteractiveUI();
	void HideInteractiveUI();
	void StartHolding();
	void StopHolding();

	UFUNCTION()
	void OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnCapsuleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);		

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION()
	void MovetoCheckpoint();

private:
	UPROPERTY(EditAnywhere, Category = "Speed Settings")
	float JumpZVelocity = true;

	UPROPERTY(EditAnywhere, Category = "Speed Settings")
	float AirControl = 0.35f;

	UPROPERTY(EditAnywhere, Category = "Speed Settings")
	float MaxWalkSpeed = 500.f;

	UPROPERTY(EditAnywhere, Category = "Speed Settings")
	float MinAnalogWalkSpeed = 20.f;

	UPROPERTY(EditAnywhere, Category = "Speed Settings")
	float BrakingDecelerationWalking = 2000.f;

	UPROPERTY(EditAnywhere, Category = "Speed Settings")
	float BrakingDecelerationFalling = 1500.0f;

	UPROPERTY()
	class AMatchaCheckpoint* CurrentCheckpoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess="true"))
	TSubclassOf<class UUserWidget> InteractiveWidgetClass;

	UPROPERTY()
	UUserWidget* InteractiveWidget;

	UPROPERTY()
	class AMatchaBaseActor* MatchaBaseActor;
	UPROPERTY()
	class AMatchaLight* MatchaLight;
	UPROPERTY()
	class AMatchaSparkle* MatchaSparkle;
	UPROPERTY()
	class AMatchaMovingPlatform* MatchaPlatform;
	UPROPERTY()
	class AMatchaBoss* MatchaBoss;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	float HoldDuration = 3.0f;

	UPROPERTY()
	float CurrentHoldDuration = 0.0f;

	FTimerHandle SkillDeathTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	class UParticleSystem* DeathEffect;

	class AMatchaGameState* GameState;

	
	
};

