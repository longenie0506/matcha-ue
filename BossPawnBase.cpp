// Fill out your copyright notice in the Description page of Project Settings.


#include "BossPawnBase.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ABossPawnBase::ABossPawnBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Component"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));

	RootComponent = CapsuleComponent;
	StaticMesh->SetupAttachment(CapsuleComponent);
}

// Called when the game starts or when spawned
void ABossPawnBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABossPawnBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABossPawnBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

UStaticMeshComponent* ABossPawnBase::GetStaticMeshComponent()
{
	return StaticMesh;
}

