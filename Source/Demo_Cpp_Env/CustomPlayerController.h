// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CustomPlayerController.generated.h"

struct FInputActionInstance;
class UInputAction;
class UInputMappingContext;
/**
 * 
 */
UCLASS()
class DEMO_CPP_ENV_API ACustomPlayerController : public APlayerController
{
	GENERATED_BODY()

#pragma region  INPUT
	
	// INPUT MAPPING CONTEXT
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Controls|Input Mapping Context", meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;
    
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Controls|Input Actions", meta=(AllowPrivateAccess = "true"))
	int32 BaseMappingPriority = 0;

	// INPUT ACTIONS
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Controls|Input Actions", meta=(AllowPrivateAccess = "true"))
	UInputAction* ShowMenuAction;
#pragma endregion

protected:
	virtual void BeginPlay();

	void HandleShowMenu(const FInputActionInstance& Instance);

	virtual void SetupInputComponent() override;
};
