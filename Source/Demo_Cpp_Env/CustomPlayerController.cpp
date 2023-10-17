// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

class UEnhancedInputLocalPlayerSubsystem;

void ACustomPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	UE_LOG(LogTemp, Warning, TEXT("Custom Player Controller Established"));
	
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
		UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}	
}

void ACustomPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("Enhanced Input Established (Player Controller)"));
		EnhancedInputComponent->BindAction(ShowMenuAction, ETriggerEvent::Started, this, &ACustomPlayerController::HandleShowMenu);
	}
}


void ACustomPlayerController::HandleShowMenu(const FInputActionInstance& Instance)
{
	UE_LOG(LogTemp, Warning, TEXT("Menu Action"));
	
	if (IsPaused())
	{
		SetPause(false);
	} else
	{
		SetPause(true);
	}
}
