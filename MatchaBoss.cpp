// Fill out your copyright notice in the Description page of Project Settings.


#include "MatchaBoss.h"
#include "BossAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SpotLightComponent.h"



AMatchaBoss::AMatchaBoss()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    //BossSpotlight = CreateDefaultSubobject<USpotLightComponent>("Spotlight");
    BossMaskFace = CreateDefaultSubobject<UStaticMeshComponent>("Mask Face");
    BossParticle = CreateDefaultSubobject<UParticleSystemComponent>("Boss Particle");

    //BossSpotlight->SetupAttachment(RootComponent);
    BossParticle->SetupAttachment(RootComponent);
    UStaticMeshComponent* BaseMeshComponent = GetStaticMeshComponent();
    if (BaseMeshComponent)
    {
        BossMaskFace->SetupAttachment(BaseMeshComponent);
    }
    
    

}

// Called when the game starts or when spawned
void AMatchaBoss::BeginPlay()
{
    Super::BeginPlay();

    PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    AIController = Cast<ABossAIController>(GetController());
    ActivateBoss();
}

// Called every frame
void AMatchaBoss::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (PlayerPawn) {
        CheckPlayerInNavBounds();
    }
    
}

void AMatchaBoss::SetBossStatus(bool bNewStatus)
{
    BossStatus = bNewStatus;
}

bool AMatchaBoss::GetBossStatus()
{
    return BossStatus;
}

void AMatchaBoss::CastSkill()
{
    
    if (WarningZoneClass && PlayerPawn)
    {
        // Spawn the warning zone
        
        FVector PlayerLocation = PlayerPawn->GetActorLocation();

        FVector GroundLocation = PlayerLocation; // Location to spawn
        FRotator SpawnRotation = PlayerPawn->GetActorRotation(); // Rotation to spawn

        FVector StartLocation = PlayerLocation; // Linetrace Start Location
        FVector EndLocation = PlayerLocation - FVector(0.0f, 0.0f, 500.0f); // Trace 500 units downwards

        FHitResult HitResult;
        FCollisionQueryParams CollisionParams;
        CollisionParams.AddIgnoredActor(this); // Ignore the boss actor

        bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, CollisionParams);

        if (bHit) {
            GroundLocation = HitResult.Location;
            SpawnRotation = FRotator::ZeroRotator;
        }

        FVector SpawnLocation = PlayerPawn->GetActorLocation();
        AMatchaWarningZone* WarningZone = GetWorld()->SpawnActor<AMatchaWarningZone>(WarningZoneClass, GroundLocation, SpawnRotation);

        if (WarningZone)
        {
            WarningZone->ShowWarning();
            FTimerDelegate TimerDel;
            TimerDel.BindLambda([this, WarningZone, GroundLocation, SpawnRotation]()
                {
                    WarningZone->HideWarning();
                    if (SkillEffectBlueprint)
                    {
                        AMatchaSkillEffect* SkillEffect = GetWorld()->SpawnActor<AMatchaSkillEffect>(SkillEffectBlueprint, GroundLocation, SpawnRotation);
                        if (SkillEffect)
                        {
                            SkillEffect->ActivateEffect();
                        }
                    }
                });
            
            GetWorld()->GetTimerManager().SetTimer(SkillTimerHandle, TimerDel, 5.0f, false);
        }
    }
}

void AMatchaBoss::CastSkills(const FSkillStruct& BossSkill)
{
    //bool Name = BossSkill.WarningZoneClass;
    if (IsValid(BossSkill.WarningZoneClass) && PlayerPawn)
    {
        FVector PlayerLocation = PlayerPawn->GetActorLocation();

        FVector GroundLocation = PlayerLocation; // Location to spawn
        FRotator SpawnRotation = PlayerPawn->GetActorRotation(); // Rotation to spawn

        FVector StartLocation = PlayerLocation; // Linetrace Start Location
        FVector EndLocation = PlayerLocation - FVector(0.0f, 0.0f, 500.0f); // Trace 500 units downwards

        FHitResult HitResult;
        FCollisionQueryParams CollisionParams;
        CollisionParams.AddIgnoredActor(this); // Ignore the boss actor

        bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, CollisionParams);

        if (bHit) {
            GroundLocation = HitResult.Location;
            SpawnRotation = FRotator::ZeroRotator;
        }

        FVector SpawnLocation = PlayerPawn->GetActorLocation();
        AMatchaWarningZone* WarningZone = GetWorld()->SpawnActor<AMatchaWarningZone>(BossSkill.WarningZoneClass, GroundLocation, SpawnRotation);

        if (WarningZone)
        {
            //WarningZone->ShowWarningAndDestroy();
            WarningZone->ShowWarning();
            FTimerDelegate TimerDel;
            
            TimerDel.BindLambda([this, WarningZone, GroundLocation, SpawnRotation, BossSkill]()
                {
                    WarningZone->HideWarning();
                    if (BossSkill.SkillEffectClass)
                    {
                        AMatchaSkillEffect* SkillEffect = GetWorld()->SpawnActor<AMatchaSkillEffect>(BossSkill.SkillEffectClass, GroundLocation, SpawnRotation);
                        if (SkillEffect)
                        {
                            SkillEffect->ActivateEffectAndDestroy(SkillEffect->GetActivationDuration());
                        }
                    }
                });

            FTimerHandle NewTimerHandle;
            GetWorld()->GetTimerManager().SetTimer(NewTimerHandle, TimerDel, WarningZone->GetWarningDuration(), false);
            SkillTimerHandles.Add(NewTimerHandle);
        }
        
    }
}

void AMatchaBoss::CastSkillByIndex(int32 SkillIndex)
{
    if (BossSkills.IsValidIndex(SkillIndex))
    {
        UE_LOG(LogTemp, Warning, TEXT("In Cast Skill Index: valid"));
        CastSkills(BossSkills[SkillIndex]);
        
    }
}

void AMatchaBoss::CastSkillsLocation(const FSkillStruct& BossSkill, FVector CastLocation)
{
    if (IsValid(BossSkill.WarningZoneClass) && PlayerPawn)
    {

        //FVector PlayerLocation = PlayerPawn->GetActorLocation();

        FVector GroundLocation = CastLocation; // Location to spawn
        FRotator SpawnRotation = PlayerPawn->GetActorRotation(); // Rotation to spawn

        FVector StartLocation = CastLocation; // Linetrace Start Location
        FVector EndLocation = CastLocation - FVector(0.0f, 0.0f, 500.0f); // Trace 500 units downwards

        FHitResult HitResult;
        FCollisionQueryParams CollisionParams;
        CollisionParams.AddIgnoredActor(this); // Ignore the boss actor

        bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, CollisionParams);

        if (bHit) {
            GroundLocation = HitResult.Location;
            SpawnRotation = FRotator::ZeroRotator;
        }

        FVector SpawnLocation = CastLocation;
        AMatchaWarningZone* WarningZone = GetWorld()->SpawnActor<AMatchaWarningZone>(BossSkill.WarningZoneClass, GroundLocation, SpawnRotation);

        if (WarningZone)
        {
            //WarningZone->ShowWarningAndDestroy();
            WarningZone->ShowWarning();
            FTimerDelegate TimerDel;

            TimerDel.BindLambda([this, WarningZone, GroundLocation, SpawnRotation, BossSkill]()
                {
                    WarningZone->HideWarning();
                    if (BossSkill.SkillEffectClass)
                    {
                        AMatchaSkillEffect* SkillEffect = GetWorld()->SpawnActor<AMatchaSkillEffect>(BossSkill.SkillEffectClass, GroundLocation, SpawnRotation);
                        if (SkillEffect)
                        {
                            SkillEffect->ActivateEffectAndDestroy(SkillEffect->GetActivationDuration());
                        }
                    }
                });

            FTimerHandle NewTimerHandle;
            GetWorld()->GetTimerManager().SetTimer(NewTimerHandle, TimerDel, WarningZone->GetWarningDuration(), false);
            SkillTimerHandles.Add(NewTimerHandle);
        }
    }
}

void AMatchaBoss::CastSkillLocationByIndex(int32 SkillIndex, FVector CastLocation)
{
    if (BossSkills.IsValidIndex(SkillIndex))
    {
        CastSkillsLocation(BossSkills[SkillIndex], CastLocation);

    }
}

void AMatchaBoss::ActivateBoss()
{
    if (AIController)
    {
        AIController->StartBehaviorTree();
    }
}

void AMatchaBoss::DeactivateBoss()
{
    if (AIController)
    {
        //AIController->StopBehaviorTree();
        SetBossStatus(false);
        AIController->SetbIsActivated(false);
        AIController->ResetAllTimers();
        ResetTimerHandle();
    }
}

void AMatchaBoss::ResetTimerHandle()
{
    for (FTimerHandle& TimerHandle : SkillTimerHandles)
    {
        GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
    }
    SkillTimerHandles.Empty();
    GetWorld()->GetTimerManager().ClearTimer(SkillTimerHandle);
    if (AIController) {
        AIController->ResetAllTimers();
    }
}

void AMatchaBoss::CheckPlayerInNavBounds()
{
    UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
    if (!NavSystem) return;
    // Check if the player location is within the NavMesh bounds
    FNavLocation NavLocation;
    bool bIsWithinNavBounds = NavSystem->GetRandomPointInNavigableRadius(PlayerPawn->GetActorLocation(), 1.0f, NavLocation);

    if (!bIsWithinNavBounds && BossStatus)
    {
        UE_LOG(LogTemp, Display, TEXT("Lose"));
        SetBossStatus(false);
    }

}

