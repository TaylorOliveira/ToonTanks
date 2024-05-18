// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraComponent.h"


ATank::ATank()
{
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm Component"));
    SpringArm->SetupAttachment(Super::BaseMesh);

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
    Camera->SetupAttachment(SpringArm);

}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent * PlayerInputComponent) {
	//SetupPlayerInputComponent(PlayerInputComponent); //Call the parent version

	// Get the player controller
    auto playerController = Cast<APlayerController>(GetController());
 
    // Get the local player enhanced input subsystem
    auto eiSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer());
    //Add the input mapping context
    eiSubsystem->AddMappingContext(inputMapping, 0);
 
    // Get the EnhancedInputComponent
    auto playerEIcomponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	//Bind Move() to the mapping
	//BindAction for enhanced system takes Action, ETriggerEvent, object, and function
	//ETriggerEvent is an enum, where Triggered means "button is held down".
	playerEIcomponent->BindAction(inputMoveForward, ETriggerEvent::Triggered, this, &ATank::Move);
    playerEIcomponent->BindAction(inputTurn, ETriggerEvent::Triggered, this, &ATank::Turn);
    playerEIcomponent->BindAction(inputRotateTurret, ETriggerEvent::Triggered, this, &ATank::RotateTurret);
    playerEIcomponent->BindAction(inputFire, ETriggerEvent::Started, this, &ABasePawn::Fire);
}

// Called every frame
void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if(TankPlayerController)
    {
        FHitResult HitResult;
        TankPlayerController->GetHitResultUnderCursor(
            ECollisionChannel::ECC_Visibility,
            false,
            HitResult);
        DrawDebugSphere(GetWorld(), HitResult.ImpactPoint,
        100.f,
        12,
        FColor::Red,
        false,
        -1.f);

        RotateTurret(HitResult.ImpactPoint);
        RotateCamera();
    }
}

void ATank::HandleDestruction()
{
    Super::HandleDestruction();
    SetActorHiddenInGame(true);
    SetActorTickEnabled(false);

    bAlive = false;
}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();

    TankPlayerController = Cast<APlayerController>(GetController());
	
}

void ATank::Move(const FInputActionValue & Value) 
{
    FVector DeltaLocation = Value.Get<FVector>();
    FVector MoveValue = Value.Get<FVector>() * Speed * UGameplayStatics::GetWorldDeltaSeconds(this);
    DeltaLocation.X = MoveValue[1];
    DeltaLocation.X = MoveValue[0];
    AddActorLocalOffset(DeltaLocation, true);
    
    //To bind to axis mapping: SetupPlayerInputComponent
	UE_LOG(LogTemp, Display, TEXT("Float value: %f"), Value.Get<float>());
}

void ATank::Turn(const FInputActionValue & Value) 
{
    FRotator DeltaRotation = FRotator::ZeroRotator;
    DeltaRotation.Yaw = (DeltaRotation.Yaw + Value.Get<float>()) * TurnRate * UGameplayStatics::GetWorldDeltaSeconds(this);
    AddActorLocalRotation(DeltaRotation, true);
	//To bind to axis mapping: SetupPlayerInputComponent
	//UE_LOG(LogTemp, Display, TEXT("Float value: %f"), Value.Get<float>());
}

void ATank::RotateTurret(const FInputActionValue & Value) 
{
    const FVector TurretRotationAxisVector = Value.Get<FVector>();
    FVector ToTarget = TurretRotationAxisVector - Super::TurretMesh->GetComponentLocation();
    //AddControllerYawInput(TurretRotationAxisVector.X);
	FRotator LookAtRotation = FRotator(0.f, ToTarget.Rotation().Yaw, 0.f);
    OutLookAtRotation = LookAtRotation;
	Super::TurretMesh->SetWorldRotation(LookAtRotation);
            //FMath::RInterpTo(
            //Super::TurretMesh->GetComponentRotation(), 
            //LookAtRotation, 
            //UGameplayStatics::GetWorldDeltaSeconds(this), 
           //60.f)
    //);

   // UE_LOG(LogTemp, Display, TEXT("Float value: %f"), ToTarget.Rotation().Yaw);
    //To bind to axis mapping: SetupPlayerInputComponent
}

void ATank::RotateCamera()
{
    //SpringArm->SetWorldRotation(
        //FMath::RInterpTo(
            //SpringArm->GetComponentRotation(),
            //FRotator(-35.f, OutLookAtRotation.Yaw, 0.f),
            //UGameplayStatics::GetWorldDeltaSeconds(this),
            //1.f)
    //);
}

