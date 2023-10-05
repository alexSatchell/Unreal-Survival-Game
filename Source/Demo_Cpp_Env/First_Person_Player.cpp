// Fill out your copyright notice in the Description page of Project Settings.
#include "First_Person_Player.h"
#include "EnhancedInputSubsystems.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Math/Vector2D.h"

// Sets default values
AFirst_Person_Player::AFirst_Person_Player()
{
	// PrimaryActorTick.bCanEverTick = true;

	// Do not rotate when controller rotates. Only rotate camera. (Free Camera)
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	// Play with this field
	GetCharacterMovement()->bOrientRotationToMovement = false; // Strafe motion. Player will not rotate
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 100.0f, 0.0f);

	// set character base variables...
	
	// Visual representation mesh 
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	StaticMeshComp->SetupAttachment(RootComponent);

	// Create camera spring arm
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComp->SetupAttachment(StaticMeshComp);
	SpringArmComp->TargetArmLength = 400.0f;
	SpringArmComp->bEnableCameraLag = true;
	SpringArmComp->CameraLagSpeed = 3.0f;
	SpringArmComp->bUsePawnControlRotation = true; // Use input (Mouse.X) to rotate the camera

	/*
     * NOTES Camera 2.0
     * Camera Needs to be aligned to center
     */
	
	// Create camera
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	// Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false; // Play with this field
		
	// Default control to this player
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

void AFirst_Person_Player::BeginPlay()
{
	Super::BeginPlay();

	// Add Input Mapping Context
	if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AFirst_Person_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFirst_Person_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &AFirst_Person_Player::HandleMovement);
    	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AFirst_Person_Player::HandleJump);
    	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AFirst_Person_Player::HandleLook);	
	}
}



#pragma region INPUT ACTION CALLBACKS
void AFirst_Person_Player::HandleMovement(const FInputActionInstance& Instance)
{
	/*
		* NOTES Handle Movement 2.0
		* -------------------------
		* Get dot product of character and camera forward vector
	    * if (camera & character forward vector is parallel i.e. Dot Product == 1)
		* {
		*	strafe movement		
		* }
		*  else if (Camera & character forward vector are not parallel i.e. dot product !=1 )
		*  {
		*	Rotate Character to match forward vector of the camera
		*	Move character in respective input direction
		*
		*	once dot product == 1, the above will be called instead
		*  }
	 */
	const FVector2D AxisValue2D = Instance.GetValue().Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddMovementInput(GetActorForwardVector() * AxisValue2D.Y);
		AddMovementInput(GetActorRightVector() * AxisValue2D.X);

	}
}

void AFirst_Person_Player::HandleLook(const FInputActionInstance& Instance)
{
	/*
		 *	NOTES Handle Look 2.0
		 *	---------------------
		 *	If (character is not moving)
		 *	{
		 *		Free look camera
		 *		Rotates around player
		 *	}
		 *
		 *	Else (character is moving)
		 *	{
		 *		Set character forward vector/rotation = to camera forward vector/rotation
		 *	}
	 */
	
	const FVector2d AxisValue2D = Instance.GetValue().Get<FVector2d>();

	if (SpringArmComp)
	{
		FRotator NewSpringArmRotation =SpringArmComp->GetComponentRotation();
		NewSpringArmRotation.Yaw += AxisValue2D.X;
		NewSpringArmRotation.Pitch += AxisValue2D.Y;

		// Limit pitch rotation
		NewSpringArmRotation.Pitch = FMath::ClampAngle(NewSpringArmRotation.Pitch, -35.0f, 60.0f);

		// Update the spring arm rotation
		SpringArmComp->SetWorldRotation(NewSpringArmRotation);
		
		// If moving - rotate the player as well
		if (GetCharacterMovement()->Velocity.SizeSquared() > 0.01f)
		{
			UE_LOG(LogTemp, Warning, TEXT("Player Moving"));
			//GetCharacterMovement()->bOrientRotationToMovement = true;
			GetCharacterMovement()->bUseControllerDesiredRotation = true;
		}
	}
}


void AFirst_Person_Player::HandleJump(const FInputActionValue& value)
{
	if (!AFirst_Person_Player::bWasJumping)
	{
		AFirst_Person_Player::Jump();
	}
}
#pragma endregion 