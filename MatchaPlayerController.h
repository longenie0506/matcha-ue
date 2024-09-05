// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MatchaPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MATCHA_API AMatchaPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
public:
//	virtual void SetupInputComponent() override;
//
//private:
//	void RestartLevel();
private:
	UPROPERTY()
	class AMatchaGameState* GameState;

	UPROPERTY()
	class AMatchaCharacter* PlayerPawn;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	//class UInputAction* RestartAction;

	//UFUNCTION()
	//void RestartGame(const FInputActionValue& Value);

	///** MappingContext */
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	//UInputMappingContext* DefaultMappingContext;
	
};
