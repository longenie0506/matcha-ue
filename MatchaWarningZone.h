// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MatchaWarningZone.generated.h"

UCLASS()
class MATCHA_API AMatchaWarningZone : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMatchaWarningZone();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void ShowWarning();

	UFUNCTION(BlueprintCallable)
	void ShowWarningAndDestroy();

	UFUNCTION(BlueprintCallable)
	void HideWarning();

	UFUNCTION(BlueprintCallable)
	void DestroyWarning();

	UFUNCTION(BlueprintPure)
	float GetWarningDuration();

private:
	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components",meta=(AllowPrivateAccess=true))
	//class UBoxComponent* WarningArea;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* WarningZoneMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = true))
	UMaterialInstance* WarningZoneMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = true))
	class UParticleSystemComponent* WarningZoneParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = true))
	float WarningDuration = 5.0f;

	// Delegate for effect finished notification
	FTimerHandle EffectTimerHandle;

};
