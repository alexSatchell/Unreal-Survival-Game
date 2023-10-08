// Fill out your copyright notice in the Description page of Project Settings.
#include "First_Person_Player.h"
#include "EnhancedInputSubsystems.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "Components/ArrowComponent.h"
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

	// Create & attach spring arm
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComp->SetupAttachment(GetRootComponent());
	SpringArmComp->TargetArmLength = 150.0f;
	SpringArmComp->bEnableCameraLag = false;
	SpringArmComp->CameraLagSpeed = 3.0f;
	SpringArmComp->bUsePawnControlRotation = true; // Use input (Mouse.X) to rotate the camera

	/*
     * NOTES Camera 2.0
     * Camera Needs to be aligned to center
     */
	
	// Create & attach camera
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false; // Play with this field

	// CameraRightShoulderLocation = CreateDefaultSubobject<UArrowComponent>(TEXT("Camera Right Shoulder"));
	// CameraRightShoulderLocation->SetupAttachment(GetMesh());

	// Offset character to the left
	FVector OriginalLocation = GetMesh()->GetRelativeLocation();
	GetMesh()->SetRelativeLocation(FVector(0.0f, -200.0f, 0.0f)); // Adjust the values as needed
	FVector NewLocation = GetMesh()->GetRelativeLocation();

	UE_LOG(LogTemp, Warning, TEXT("Original Location: %s"), *OriginalLocation.ToString());
	UE_LOG(LogTemp, Warning, TEXT("New Location: %s"), *NewLocation.ToString());
	
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
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &AFirst_Person_Player::HandleFire);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Triggered, this, &AFirst_Person_Player::HandleAim);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &AFirst_Person_Player::HandleSprint);
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

void AFirst_Person_Player::HandleSprint(const FInputActionValue& value)
{
	UE_LOG(LogTemp, Warning, TEXT("Sprint"));
}

void AFirst_Person_Player::HandleAim(const FInputActionValue& value)
{
	/*
		 * NOTES Handle Aim
		 * ----------------
		 * Player Rotation
		 * Get dot product of player and camera
		 * if (character is not facing camera i.e dot product != 1)
		 * {
		 *		SNAP-Rotate character to face same direction as camera
		 *		perform ads action 
		 * }
	 */
	UE_LOG(LogTemp, Warning, TEXT("Aim"));
}

void AFirst_Person_Player::HandleFire(const FInputActionValue& value)
{
	/*
		 * NOTES Handle Shoot 
		 * ------------------
		 *	Player Rotation
		 *	Get dot product of player and camera
		 *	If (character is not facing camera i.e dot product != 1)
		 *	{
		 *		SNAP-Rotate character to face same direction as camera
		 *		perform shoot action
		 *	}
	 */

	// Ray cast into the world
	FVector StartLocation = GetMesh()->GetComponentLocation();
	FRotator ViewRotation = GetMesh()->GetComponentRotation();
	FVector EndLocation = StartLocation + (ViewRotation.Vector() * 1500.0f);

	// Perform line trace
	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	if (bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		StartLocation,
		EndLocation,
		ECC_Visibility,
		CollisionParams
	))
	{
		DrawDebugLine(
			GetWorld(),
			StartLocation,
			HitResult.ImpactPoint,
			FColor::Orange,
			false,
			1,
			0,
			1
		);
		UE_LOG(LogTemp, Warning, TEXT("Shoot"));
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