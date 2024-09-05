// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "StoryStruct.h"
#include "StoryObject.h"
#include "MatchaGameState.generated.h"


UENUM(BlueprintType)
enum class EChallengeState : uint8 {
	ECS_None UMETA(DisplayName = "None"),
	ECS_Challenge1 UMETA(DisplayName = "Challenge 1"),
	ECS_Challenge2 UMETA(DisplayName = "Challenge 2"),
	ECS_Challenge3 UMETA(DisplayName = "Challenge 3"),
	ECS_Completed UMETA(DisplayName = "Completed")

};

USTRUCT(BlueprintType)
struct FStoryLine {
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Storyline")
	UTexture2D* Image;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Storyline")
	FString Text;
};

/**
 * 
 */
UCLASS()
class MATCHA_API AMatchaGameState : public AGameStateBase
{
	GENERATED_BODY()
public: 
	AMatchaGameState();

	UPROPERTY(BlueprintReadWrite, Category = "Challenges")
	EChallengeState CurrentChallenge;

	// Challenge 1
	UFUNCTION()
	void IncrementLightTurnedOn();

	int32 GetTotalLight();
	void SetTotalLight(int32 newTotalLight);

	int32 GetTotalLightTurnedOn();
	void SetTotalLightTurnedOn(int32 newTotalLightTurnedOn);

	void CountTotalLightTurnedOn();

	virtual void BeginPlay() override;

	// Challenge 2
	UFUNCTION()
	void IncrementSparkleCollected();

	virtual void Tick(float DeltaTime) override;

	bool GetBossChallengeStatus();

	void SetBossChallengeStatus(bool bNewStatus);

	void ResetChallenge3();

	bool GetIsEndGame();


	FText GetFormattedCountdownTime() const;

	FText GetFormattedCurrentLight() const;

	FText GetFormattedCurrentSparkle() const;

	void UpdateChallengeWidget(FText ChallengeText);

// Widget
public:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UUserWidget> TimerWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UUserWidget> StorylineWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UUserWidget> EndWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UUserWidget> ChallengeWidgetClass;

	UPROPERTY()
	UUserWidget* TimerWidget;

	UPROPERTY()
	UUserWidget* StorylineWidget;

	UPROPERTY()
	UUserWidget* EndgameWidget;

	UPROPERTY()
	UUserWidget* ChallengeWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TArray<TSubclassOf<UStoryObject>> Storylines;

	UPROPERTY()
	UStoryObject* CurrentStory;
	
	UPROPERTY()
	int32 CurrentStoryStruct;

	UPROPERTY()
	int32 CurrentStoryIndex;

	UPROPERTY()
	bool bIsInStory;

	UFUNCTION(BlueprintCallable, Category = "Story")
	void UpdateStory();

	void ShowStoryline();

	void ReplayStoryline();

	UFUNCTION()
	void OnNextStory();

	void ShowEndgame();

	UFUNCTION(BlueprintCallable)
	void SetTimerText(FText NewText);

	//void ShowStoryline(FString StoryText, UTexture2D* StoryImage);
	//void HideStoryline();

private:

	// Challenge 1
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Challenge 1: Light State", meta = (AllowPrivateAccess=true))
	int32 TotalLight = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Challenge 1: Light State", meta = (AllowPrivateAccess = true))
	int32 TotalLightTurnedOn = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Challenge 1: Light State", meta = (AllowPrivateAccess = true))
	int32 TotalLightToComplete = 5;

	void TurnOffRandomLight(int32 totalLightToComplete);

	// Challenge 2

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Challenge 2: Sparkle State", meta = (AllowPrivateAccess = true))
	int32 TotalSparkle = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Challenge 2: Sparkle State", meta = (AllowPrivateAccess = true))
	int32 CollectedSparkled = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Challenge 2: Sparkle State", meta = (AllowPrivateAccess = true))
	int32 TotalSparkleToComplete = 5;

	// Challenge 3
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Challenge 3: Boss", meta = (AllowPrivateAccess = true))
	float BossCountdownTime = 60;

	UPROPERTY()
	float BossCountdownCurrentTime = 0;

	UPROPERTY()
	bool bIsChallenge3 = false;

	UPROPERTY()
	bool bIsEndgame = false;

	// Function

	void StartChallenge1();

	void StartChallenge2();

	void StartChallenge3();

	void UpdateCountdown(float DeltaTime);

	UPROPERTY()
	class APlayerController* PlayerController;

	class AMatchaBoss* MatchaBossInstance;

	

public: 

	void ShowCursor();

	void HideCursor();

	UFUNCTION(BlueprintCallable, Category = "Game")
	void HandleEndGame(bool bIsWin);
	void ResetActorState();

};
