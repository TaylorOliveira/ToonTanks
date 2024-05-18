// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "Tank.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API ATank : public ABasePawn
{ 
	GENERATED_BODY()
	
public:
	ATank();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void HandleDestruction();

	APlayerController* GetTankPlayerController() const {return TankPlayerController;}

	bool bAlive = true;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
 	class USpringArmComponent* SpringArm;

 	UPROPERTY(VisibleAnywhere, Category = "Components")
 	class UCameraComponent* Camera;
 	
	//Setting up inputs. Mapping context and Input Action variables, to be set in Blueprint
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
    class UInputMappingContext * inputMapping;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction * inputMoveForward;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction * inputTurn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction * inputRotateTurret;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction * inputFire;

	FRotator OutLookAtRotation;
	
	void Move(const FInputActionValue & Value);

	void Turn(const FInputActionValue & Value);

	void RotateTurret(const FInputActionValue & Value);

	void RotateCamera();

	FHitResult TrackMouse(FHitResult HitResult);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Move Settings", meta = (AllowPrivateAccess = "true"))
	float Speed = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Move Settings", meta = (AllowPrivateAccess = "true"))
	float TurnRate = 100.f;

	void SetupPlayerInputComponent(UInputComponent * PlayerInputComponent);

	APlayerController* TankPlayerController;
};

