// Fill out your copyright notice in the Description page of Project Settings.
#include "First_Person_Player.h"
#include "EnhancedInputSubsystems.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "Components/CapsuleComponent.h"
#include "Math/Vector2D.h"

// Sets default values
AFirst_Person_Player::AFirst_Person_Player()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create Subobjects/Actor Components
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	
	// Setup Attachments
	if (UCapsuleComponent* RootComp = GetCapsuleComponent())
	{
		UE_LOG(LogTemp, Warning, TEXT("Made It"));
		StaticMeshComp->SetupAttachment(RootComp);
		SpringArmComp->SetupAttachment(StaticMeshComp);
		CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	}

	// Spring Arm Setup
	SpringArmComp->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 50.0f), FRotator(-60.f, 0.0f,0.0f));
	SpringArmComp->TargetArmLength = 400.0f;
	SpringArmComp->bEnableCameraLag = true;
	SpringArmComp->CameraLagSpeed = 3.0f;
		
	// Default control to this player
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void AFirst_Person_Player::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AFirst_Person_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AFirst_Person_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Get player controller
	const auto PlayerController = Cast<APlayerController>(GetController());

	// Get local player input system
	const auto InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

	// Add input mapping context
	InputSubsystem->AddMappingContext(BaseMappingContext, BaseMappingPriority);

	// Get the enhanced input component
	const auto PlayerInputComp = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	PlayerInputComp->BindAction(MovementAction, ETriggerEvent::Triggered, this, &AFirst_Person_Player::HandleMovement);
	PlayerInputComp->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AFirst_Person_Player::HandleJump);
	PlayerInputComp->BindAction(LookAction, ETriggerEvent::Triggered, this, &AFirst_Person_Player::HandleLook);
}

#pragma region IPUT ACTION CALLBACKS

void AFirst_Person_Player::HandleMovement(const FInputActionInstance& Instance)
{
	const FVector2D AxisValue2D = Instance.GetValue().Get<FVector2D>();
	AddMovementInput(GetActorForwardVector() * AxisValue2D.Y);
	AddMovementInput(GetActorRightVector() * AxisValue2D.X);
}

void AFirst_Person_Player::HandleLook(const FInputActionInstance& Instance)
{
	const FVector2d AxisValue2D = Instance.GetValue().Get<FVector2d>();

	// If standing - rotate camera around player
	if (SpringArmComp)
	{
		FRotator NewSpringArmRotation =SpringArmComp->GetComponentRotation();
		NewSpringArmRotation.Yaw += AxisValue2D.X;
		NewSpringArmRotation.Pitch += AxisValue2D.Y;

		// Limit pitch rotation
		NewSpringArmRotation.Pitch = FMath::ClampAngle(NewSpringArmRotation.Pitch, -30.0f, 60.0f);

		// Update the spring arm rotation
		SpringArmComp->SetWorldRotation(NewSpringArmRotation);
	}

	// If moving - rotate the player
	
	//AddControllerYawInput(AxisValue2D.X);
	//AddControllerPitchInput(AxisValue2D.Y);
}


void AFirst_Person_Player::HandleJump(const FInputActionValue& value) {}






