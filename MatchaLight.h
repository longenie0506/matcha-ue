// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MatchaBaseActor.h"
#include "MatchaLight.generated.h"

UCLASS()
class MATCHA_API AMatchaLight : public AMatchaBaseActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMatchaLight();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool GetLightStatus();

	void SetLightStatus(bool bNewLightStatus);

private:
	UPROPERTY()
	bool bLightStatus = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Component",meta=(AllowPrivateAccess=true))
	class UPointLightComponent* LightComponent;


	void NotifyGameState();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USoundBase* LightSound;

};
