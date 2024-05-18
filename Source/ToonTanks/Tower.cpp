// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

void ATower::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if(InFireRange())
    {   
        FVector ToTank = Tank->GetActorLocation() - Super::TurretMesh->GetComponentLocation();
        //UE_LOG(LogTemp, Display, TEXT("Tank Detected"));
        RotateTower(ToTank);

    }
}

void ATower::BeginPlay()
{
    Super::BeginPlay();

    Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));

    GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ATower::CheckFireCondition, FireRate, true);
}

void ATower::CheckFireCondition()
{
    if(Tank == nullptr)
    {
        return;
    }
    
    if(InFireRange() && Tank->bAlive)
    {
    Super::Fire();
    }
}

void ATower::RotateTower(FVector PawnVector)
{   
    FRotator TowerRotation =FRotator(0.f, PawnVector.Rotation().Yaw, 0.f);
    Super::TurretMesh->SetWorldRotation(
        FMath::RInterpTo(
            Super::TurretMesh->GetComponentRotation(), 
            TowerRotation, 
            UGameplayStatics::GetWorldDeltaSeconds(this), 
            10.f)
    );
    //UE_LOG(LogTemp, Display, TEXT("TowerRotation = %f"), PawnVector.Rotation().Yaw);
}

bool ATower::InFireRange()
{
    if(Tank)
    {
        float Distance = FVector::Dist(GetActorLocation(),Tank->GetActorLocation());
        if(Distance <= FireRange)
        {
            return true;
        }
    }

    return false;
}

void ATower::HandleDestruction()
{
    Super::HandleDestruction();
    Destroy();
}