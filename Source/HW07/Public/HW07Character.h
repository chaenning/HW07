// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "HW07PlayerController.h"
#include "HW07Character.generated.h"

class UCapsuleComponent;
class USpringArmComponent; 
class UCameraComponent;
struct FInputActionValue;

UCLASS()
class HW07_API AHW07Character : public APawn
{
	GENERATED_BODY()

public:
	AHW07Character();

protected:

	AHW07PlayerController* HW07Cotroller;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Component")
	UCapsuleComponent* CapsuleComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Component")
	USkeletalMeshComponent* SkeletalMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComp;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void Move(const FInputActionValue& value);
	UFUNCTION()
	void Look(const FInputActionValue& value);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	float MoveSpeed = 300.f; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	float LookSpeed = 2.0;
};
