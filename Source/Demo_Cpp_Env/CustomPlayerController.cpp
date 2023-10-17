// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomPlayerController.h"

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
