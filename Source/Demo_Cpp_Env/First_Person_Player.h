// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputMappingContext.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "First_Person_Player.generated.h"

class UInputAction;
class UInputMappingContext;

UCLASS()
class DEMO_CPP_ENV_API AFirst_Person_Player : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFirst_Person_Player();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	#pragma region Camera
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Camera|Spring Arm")
	USpringArmComponent* SpringArmComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Camera|Camera")
	UCameraComponent* CameraComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Camera|Mesh")
	UStaticMeshComponent* StaticMeshComp;
	#pragma endregion
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	#pragma region INPUT
	
	// INPUT MAPPING CONTEXT
	// ---------------------
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Controls|Input Actions")
	UInputMappingContext* BaseMappingContext;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Controls|Input Actions")
	int32 BaseMappingPriority = 0;
	
	// INPUT ACTIONS
	// -------------
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Controls|Input Actions")
	UInputAction* MovementAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Controls|Input Actions")
	UInputAction* LookAction;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Controls|Input Actions")
	UInputAction* JumpAction;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Controls|Input Actions")
	UInputAction* FireAction;

	// INPUT FUNCTIONS
	// ---------------
	void HandleMovement(const FInputActionInstance& Instance);

	void HandleLook(const FInputActionInstance& Instance);

	void HandleJump(const FInputActionValue& value);

	void HandleFire(const FInputActionValue& value);

#pragma endregion 
};


