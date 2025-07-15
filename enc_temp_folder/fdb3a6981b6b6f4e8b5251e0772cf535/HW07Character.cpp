#include "HW07Character.h"
#include "HW07PlayerController.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"

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
            HW07Cotroller = PlayerController;
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

 /*   if (!FMath::IsNearlyZero(MoveInput.X) || !FMath::IsNearlyZero(MoveInput.Y))
    {*/
        FRotator Rotation = HW07Cotroller->GetControlRotation();
        UE_LOG(LogTemp, Warning, TEXT("%s"), *Rotation.ToString());

        const FRotator YawRotation(0, Rotation.Yaw, 0);
        // get forward vector
        const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        // get right vector 
        const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

        // add movement 
        AddMovementInput(ForwardDirection, MoveInput.Y);
        AddMovementInput(RightDirection, MoveInput.X);

        
    //}
}


 void AHW07Character::Look(const FInputActionValue& value)
 {
     FVector2D LookInput = value.Get<FVector2D>();

     if (!FMath::IsNearlyZero(LookInput.X))
     {
         FRotator YawRotation(0.f, LookInput.X, 0.f);
         AddActorLocalRotation(YawRotation);
     }

     FRotator CurrentRotation = SpringArmComp->GetRelativeRotation();
     float NewPitch = FMath::Clamp(CurrentRotation.Pitch + LookInput.Y, -80.f, 80.f);
     SpringArmComp->SetRelativeRotation(FRotator(NewPitch, CurrentRotation.Yaw, 0.f));
 }