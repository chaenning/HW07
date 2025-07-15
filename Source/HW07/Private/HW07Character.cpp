#include "HW07Character.h"
#include "HW07PlayerController.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AHW07Character::AHW07Character()
{
	PrimaryActorTick.bCanEverTick = false;

    CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
    SetRootComponent(CapsuleComp);

    SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal"));
    SkeletalMeshComp->SetupAttachment(CapsuleComp);

    SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArmComp->SetupAttachment(CapsuleComp);
    SpringArmComp->TargetArmLength = 300.0f;
    SpringArmComp->bUsePawnControlRotation = false;

    CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
    CameraComp->bUsePawnControlRotation = false;
}


void AHW07Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {

        if (AHW07PlayerController* PlayerController = Cast<AHW07PlayerController>(GetController()))
        {
            if (PlayerController->MoveAction)
            {
                EnhancedInput->BindAction(
                    PlayerController->MoveAction,
                    ETriggerEvent::Triggered,
                    this,
                    &AHW07Character::Move
                );
            }

            if (PlayerController->LookAction)
            {
                EnhancedInput->BindAction(
                    PlayerController->LookAction,
                    ETriggerEvent::Triggered,
                    this,
                    &AHW07Character::Look
                );
            }
        }
    }
}

void AHW07Character::Move(const FInputActionValue & value)
{
    if (!Controller) return;

    const FVector2D MoveInput = value.Get<FVector2D>();

    if (!FMath::IsNearlyZero(MoveInput.X) || !FMath::IsNearlyZero(MoveInput.Y))
    {
        FVector Forward = GetActorForwardVector();
        FVector Right = GetActorRightVector();

        FVector MoveDirection = Forward * MoveInput.X + Right * MoveInput.Y;

        AddActorLocalOffset(MoveDirection.GetSafeNormal() * MoveSpeed * GetWorld()->GetDeltaSeconds(), true);
    }
}


 void AHW07Character::Look(const FInputActionValue& value)
 {
     FVector2D LookInput = value.Get<FVector2D>();

     float Yaw = LookInput.X;
     float Pitch= LookInput.Y;

     if (SpringArmComp)
     {
         FRotator CurrentRotation = SpringArmComp->GetRelativeRotation();
         
         float NewYaw = CurrentRotation.Yaw + Yaw;
         float NewPitch = FMath::Clamp(CurrentRotation.Pitch + Pitch, -80.f, 80.f);

         SpringArmComp->SetRelativeRotation(FRotator(NewPitch, NewYaw, 0.f));
     }
 }