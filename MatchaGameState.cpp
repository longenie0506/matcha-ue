// Fill out your copyright notice in the Description page of Project Settings.


#include "MatchaGameState.h"
#include "MatchaLight.h"
#include "MatchaSparkle.h"
#include "MatchaBoss.h"
#include "MatchaSkillEffect.h"
#include "MatchaWarningZone.h"
#include "MatchaMovingPlatform.h"
#include "MatchaCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Engine/World.h"
#include "Algo/RandomShuffle.h"

AMatchaGameState::AMatchaGameState()
{
	PrimaryActorTick.bCanEverTick = true;
	CurrentChallenge = EChallengeState::ECS_None;
}

void AMatchaGameState::BeginPlay()
{
	Super::BeginPlay();

	CurrentStoryStruct = 0;
	CurrentStoryIndex = 0;

	MatchaBossInstance = Cast<AMatchaBoss>(UGameplayStatics::GetActorOfClass(GetWorld(), AMatchaBoss::StaticClass()));

	if (TimerWidgetClass)
	{
		TimerWidget = CreateWidget<UUserWidget>(GetWorld(), TimerWidgetClass);
	}
	if (EndWidgetClass)
	{
		EndgameWidget = CreateWidget<UUserWidget>(GetWorld(), EndWidgetClass);
	}
	if (ChallengeWidgetClass)
	{
		ChallengeWidget = CreateWidget<UUserWidget>(GetWorld(), ChallengeWidgetClass);
	}
	if (StorylineWidgetClass)
	{
		StorylineWidget = CreateWidget<UUserWidget>(GetWorld(), StorylineWidgetClass);
		if (StorylineWidget)
		{
			// Bind the OnClicked event of the Button to OnNextStory
			UButton* Button = Cast<UButton>(StorylineWidget->GetWidgetFromName("Button_443"));
			if (Button)
			{
				Button->OnClicked.AddDynamic(this, &AMatchaGameState::OnNextStory);
			}
			CurrentStoryIndex = 0;
			ShowStoryline();
		}
	}
	PlayerController = GetWorld()->GetFirstPlayerController();

	/*PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	if (PlayerController)
	{
		PlayerControlle
r->SetShowMouseCursor(true);
		PlayerController->EnableInput(PlayerController);
	}*/

	StartChallenge1();
	ShowCursor();
	//StartChallenge3();
	//ShowEndgame();
}

void AMatchaGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsChallenge3)
	{
		UpdateCountdown(DeltaTime);
		SetTimerText(GetFormattedCountdownTime());
	}
	if (CurrentChallenge == EChallengeState::ECS_Challenge1) {
		UpdateChallengeWidget(GetFormattedCurrentLight());
	}else if (CurrentChallenge == EChallengeState::ECS_Challenge2) {
		UpdateChallengeWidget(GetFormattedCurrentSparkle());
	}
}

bool AMatchaGameState::GetBossChallengeStatus()
{
	return bIsChallenge3;
}

void AMatchaGameState::SetBossChallengeStatus(bool bNewStatus)
{
	bIsChallenge3 = bNewStatus;
	if (bIsChallenge3) {
		if (TimerWidget)
		{
			TimerWidget->AddToViewport();
		}
	}
	else {
		if (TimerWidget)
		{
			TimerWidget->RemoveFromParent();
		}
	}
}


void AMatchaGameState::StartChallenge1()
{
	CurrentChallenge = EChallengeState::ECS_Challenge1;
	TurnOffRandomLight(TotalLightToComplete);
	
	/*if (ChallengeWidget) {
		ChallengeWidget->AddToViewport(1);
	}*/
}

void AMatchaGameState::StartChallenge2()
{
	UE_LOG(LogTemp, Warning, TEXT("Currently in Challenge 2"));
	CurrentChallenge = EChallengeState::ECS_Challenge2;
	/*if (ChallengeWidget) {
		ChallengeWidget->AddToViewport(1);
	}*/
	UpdateStory();

	TArray<AActor*> SparkleArray;
	UGameplayStatics::GetAllActorsOfClass(this, AMatchaSparkle::StaticClass(), SparkleArray);
	TotalSparkle = SparkleArray.Num();
	CollectedSparkled = 0;
	if (TotalSparkleToComplete <= TotalLight) {
		UE_LOG(LogTemp, Warning, TEXT("Successfully spawn sparkles: %d"), TotalSparkleToComplete);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Not enough sparkles to spawn"));
		return;
	}

	Algo::RandomShuffle(SparkleArray);

	for (int32 i = 0; i < TotalSparkleToComplete; ++i)
	{
		AMatchaSparkle* Sparkle = Cast<AMatchaSparkle>(SparkleArray[i]);
		if (Sparkle)
		{
			Sparkle->SetActorHiddenInGame(false);
			Sparkle->SetActorEnableCollision(true);
			Sparkle->SetSparkleStatus(false);  // Make sure the sparkle is spawn
		}
	}

}

void AMatchaGameState::StartChallenge3() {
	UE_LOG(LogTemp, Warning, TEXT("Currently in Challenge 3"));
	CurrentChallenge = EChallengeState::ECS_Challenge3;
	/*if (ChallengeWidget) {
		ChallengeWidget->RemoveFromParent();
	}*/
	UpdateStory();

	TArray<AActor*> MovingPlatformArray;
	UGameplayStatics::GetAllActorsOfClass(this, AMatchaMovingPlatform::StaticClass(), MovingPlatformArray);
	
	for (int32 i = 0; i < MovingPlatformArray.Num(); i++) {
		AMatchaMovingPlatform* MovingPlatform = Cast<AMatchaMovingPlatform>(MovingPlatformArray[i]);
		if (MovingPlatform) {
			MovingPlatform->SetActorHiddenInGame(false);
			MovingPlatform->SetActorEnableCollision(true);
		}
	}

}

void AMatchaGameState::UpdateCountdown(float DeltaTime)
{
	BossCountdownCurrentTime -= DeltaTime;
	if (BossCountdownCurrentTime <= 0.0f)
	{
		BossCountdownCurrentTime = 0.0f;
		ResetActorState();
		HandleEndGame(true);
	}
}

void AMatchaGameState::ShowCursor()
{
	if (PlayerController)
	{
		// Show the mouse cursor
		PlayerController->bShowMouseCursor = true;

		// Set the input mode to UI only
		FInputModeUIOnly InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		PlayerController->SetInputMode(InputMode);
	}
}

void AMatchaGameState::HideCursor()
{
	if (PlayerController)
	{
		// Hide the mouse cursor
		PlayerController->bShowMouseCursor = false;

		// Set the input mode to game only
		FInputModeGameOnly InputMode;
		PlayerController->SetInputMode(InputMode);
	}
}

void AMatchaGameState::HandleEndGame(bool bIsWin)
{
	if (MatchaBossInstance)
	{
		bIsChallenge3 = false;
		MatchaBossInstance->DeactivateBoss();
		if (bIsWin) {
			UpdateStory();
		}
		
	}

	
}

void AMatchaGameState::ResetActorState()
{
	SetBossChallengeStatus(false);

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMatchaSkillEffect::StaticClass(), FoundActors);

	for (AActor* Actor : FoundActors)
	{
		if (Actor)
		{

			Actor->Destroy();
		}
	}

	TArray<AActor*> FoundActorsBoss;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMatchaBoss::StaticClass(), FoundActorsBoss);

	for (AActor* Actor : FoundActorsBoss)
	{
		if (Actor)
		{
			AMatchaBoss* Boss = Cast<AMatchaBoss>(Actor);
			Boss->ResetTimerHandle();
		}
	}

	TArray<AActor*> FoundActorsZone;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMatchaWarningZone::StaticClass(), FoundActorsZone);

	for (AActor* Actor : FoundActorsZone)
	{
		if (Actor)
		{
			Actor->Destroy();
		}
	}

}

void AMatchaGameState::ResetChallenge3()
{
	CurrentChallenge = EChallengeState::ECS_Challenge3;
	BossCountdownCurrentTime = BossCountdownTime;
	bIsChallenge3 = true;
	if (TimerWidget)
	{
		TimerWidget->AddToViewport();
	}
	
}

bool AMatchaGameState::GetIsEndGame()
{
	return bIsEndgame;
}

void AMatchaGameState::TurnOffRandomLight(int32 totalLightToComplete)
{
	TArray<AActor*> LightArray;
	UGameplayStatics::GetAllActorsOfClass(this, AMatchaLight::StaticClass(), LightArray);
	TotalLight = LightArray.Num();
	TotalLightTurnedOn = TotalLight - TotalLightToComplete;

	if (totalLightToComplete <= TotalLight) {
		UE_LOG(LogTemp, Warning, TEXT("Successfully create %d turned-off lights"), TotalLightToComplete);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Not enough lights to turn them off"));
		return;
	}

	Algo::RandomShuffle(LightArray);

	for (int32 i = 0; i < totalLightToComplete; ++i)
	{
		AMatchaLight* Light = Cast<AMatchaLight>(LightArray[i]);
		if (Light)
		{
			Light->SetLightStatus(false);  // Make sure the light is turned off
		}
	}

	CountTotalLightTurnedOn();
}




void AMatchaGameState::IncrementLightTurnedOn()
{
	TotalLightTurnedOn++;

	if (TotalLightTurnedOn >= TotalLight) {
		UE_LOG(LogTemp, Warning, TEXT("Challenge Completed"));
		StartChallenge2();
	}
}

void AMatchaGameState::IncrementSparkleCollected()
{
	CollectedSparkled++;
	UE_LOG(LogTemp, Warning, TEXT("Sparkles: %d"), CollectedSparkled);

	if (CollectedSparkled >= TotalSparkleToComplete) {
		UE_LOG(LogTemp, Warning, TEXT("Challenge Completed"));
		StartChallenge3();
	}
}

int32 AMatchaGameState::GetTotalLight()
{
	return TotalLight;
}

void AMatchaGameState::SetTotalLight(int32 newTotalLight)
{
	TotalLight = newTotalLight;
}

int32 AMatchaGameState::GetTotalLightTurnedOn()
{
	return TotalLightTurnedOn;
}

void AMatchaGameState::SetTotalLightTurnedOn(int32 newTotalLightTurnedOn)
{
	TotalLightTurnedOn = newTotalLightTurnedOn;
}

void AMatchaGameState::CountTotalLightTurnedOn()
{
	TArray<AActor*> LightArray;
	UGameplayStatics::GetAllActorsOfClass(this, AMatchaLight::StaticClass(), LightArray);
	TotalLightTurnedOn = 0;
	for (AActor* iActor : LightArray) {
		AMatchaLight* MatchaLight = Cast<AMatchaLight>(iActor);
		if (MatchaLight && MatchaLight->GetLightStatus()) {
			TotalLightTurnedOn++;
		}
	}
}

FText AMatchaGameState::GetFormattedCountdownTime() const
{
	int32 Minutes = FMath::FloorToInt(static_cast<float>(BossCountdownCurrentTime) / 60.0f);
	int32 Seconds = FMath::FloorToInt(static_cast<float>(BossCountdownCurrentTime)) % 60;
	return FText::Format(NSLOCTEXT("YourNamespace", "CountdownFormat", "{0}:{1}"),
		FText::AsNumber(Minutes),
		FText::AsNumber(Seconds));
}

FText AMatchaGameState::GetFormattedCurrentLight() const
{
	return FText::Format(NSLOCTEXT("YourNamespace", "CountdownFormat", "{0}/{1}"),
		FText::AsNumber(TotalLight-TotalLightTurnedOn),
		FText::AsNumber(TotalLightToComplete));
}

FText AMatchaGameState::GetFormattedCurrentSparkle() const
{
	return FText::Format(NSLOCTEXT("YourNamespace", "CountdownFormat", "{0}/{1}"),
		FText::AsNumber(CollectedSparkled),
		FText::AsNumber(TotalSparkleToComplete));
}

void AMatchaGameState::UpdateChallengeWidget(FText ChallengeText)
{
	if (ChallengeWidget)
	{
		UTextBlock* TimerText = Cast<UTextBlock>(ChallengeWidget->GetWidgetFromName("TextBlock_0"));
		if (TimerText)
		{
			TimerText->SetText(ChallengeText);
		}
	}
}

void AMatchaGameState::SetTimerText(FText NewText)
{
	if (TimerWidget)
	{
		UTextBlock* TimerText = Cast<UTextBlock>(TimerWidget->GetWidgetFromName("TextBlock_0"));
		if (TimerText)
		{
			TimerText->SetText(GetFormattedCountdownTime());
		}
	}
}

void AMatchaGameState::UpdateStory()
{
	UE_LOG(LogTemp, Display, TEXT("%d"), CurrentStoryStruct);
	UE_LOG(LogTemp, Display, TEXT("%d"), Storylines.Num());
	if (CurrentStoryStruct+1<Storylines.Num()) {
		UE_LOG(LogTemp, Display, TEXT("Current in story no %d, move on to story no %d"), CurrentStoryStruct, CurrentStoryStruct + 1);
		CurrentStoryStruct++;
		CurrentStoryIndex = 0;
		bIsInStory = true;
		ShowCursor();
		ShowStoryline();
	}
	else {
		UE_LOG(LogTemp, Display, TEXT("Current in story no %d, can't move on to next story"), CurrentStoryStruct);
	}
	
}

void AMatchaGameState::ShowStoryline()
{
	if (StorylineWidget && Storylines.IsValidIndex(CurrentStoryStruct))
	{	
		CurrentStory = NewObject<UStoryObject>(this, Storylines[CurrentStoryStruct]);
		
		if (CurrentStory->FullStorySegment.IsValidIndex(CurrentStoryIndex)) {
			UTextBlock* TextBlock = Cast<UTextBlock>(StorylineWidget->GetWidgetFromName("TextBlock_0"));
			UImage* ImageBlock = Cast<UImage>(StorylineWidget->GetWidgetFromName("Image_0"));
			if (TextBlock)
			{
				TextBlock->SetText(FText::FromString(CurrentStory->FullStorySegment[CurrentStoryIndex].Text));
			}

			if (ImageBlock && CurrentStory->FullStorySegment[CurrentStoryIndex].Image)
			{
				ImageBlock->SetBrushFromTexture(CurrentStory->FullStorySegment[CurrentStoryIndex].Image);
			}
			if (CurrentStoryIndex == 0) {
				StorylineWidget->AddToViewport();
			}
			
		}
	}
}

void AMatchaGameState::ReplayStoryline()
{
	CurrentStoryIndex = 0;
	ShowStoryline();
}

void AMatchaGameState::OnNextStory()
{
	CurrentStoryIndex++;
	if (CurrentStory && CurrentStoryIndex < CurrentStory->FullStorySegment.Num())
	{
		ShowStoryline();
	}
	else
	{
		// Hide the storyline widget or do something else when the story ends
		StorylineWidget->RemoveFromParent();
		bIsInStory = false;
		HideCursor();
		if (CurrentStoryStruct + 1 == Storylines.Num()) {
			ShowEndgame();
		}
	}
}

void AMatchaGameState::ShowEndgame()
{
	if (EndgameWidget) {
		EndgameWidget->AddToViewport();

		if (PlayerController)
		{
			PlayerController->SetShowMouseCursor(true);
			PlayerController->EnableInput(PlayerController);

			FInputModeGameAndUI InputMode;
			PlayerController->SetInputMode(InputMode);
			PlayerController->bShowMouseCursor = true; // or false, depending on your game's needs
		}
	}

	
}
