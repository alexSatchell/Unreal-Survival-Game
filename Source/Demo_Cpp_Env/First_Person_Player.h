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

	#pragma region CAMERA
    	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Camera|Spring Arm", meta=(AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArmComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Camera|Camera", meta=(AllowPrivateAccess = "true"))
	UCameraComponent* CameraComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Camera|Mesh", meta=(AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMeshComp;
	
	#pragma endregion
	
	#pragma region INPUT
	
	// INPUT MAPPING CONTEXT
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Controls|Input Actions", meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Controls|Input Actions", meta=(AllowPrivateAccess = "true"))
	int32 BaseMappingPriority = 0;
	
	// INPUT ACTIONS
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Controls|Input Actions", meta=(AllowPrivateAccess = "true"))
	UInputAction* MovementAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Controls|Input Actions", meta=(AllowPrivateAccess = "true"))
	UInputAction* LookAction;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Controls|Input Actions", meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Controls|Input Actions", meta=(AllowPrivateAccess = "true"))
	UInputAction* FireAction;
	
	#pragma endregion
	
public:
	// Sets default values for this character's properties
	AFirst_Person_Player();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// INPUT FUNCTIONS
	void HandleMovement(const FInputActionInstance& Instance);

	void HandleLook(const FInputActionInstance& Instance);

	void HandleJump(const FInputActionValue& value);

	void HandleFire(const FInputActionValue& value);

	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
};


