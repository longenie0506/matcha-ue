// Fill out your copyright notice in the Description page of Project Settings.


#include "MatchaBaseActor.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AMatchaBaseActor::AMatchaBaseActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Component"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));

	RootComponent = CapsuleComponent;
	StaticMesh->SetupAttachment(CapsuleComponent);
}

// Called when the game starts or when spawned
void AMatchaBaseActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMatchaBaseActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UStaticMeshComponent* AMatchaBaseActor::GetStaticMeshComponent()
{
	return StaticMesh;
}



