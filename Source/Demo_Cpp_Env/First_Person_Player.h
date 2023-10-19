// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputMappingContext.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "First_Person_Player.generated.h"

class UInputAction;
class UInputMappingContext;

UCLASS()
class DEMO_CPP_ENV_API AFirst_Person_Player : public ACharacter
{
	GENERATED_BODY()

#pragma region INPUT

	// INPUT MAPPING CONTEXT
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Controls|Input Mapping Context",
		meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Controls|Input Actions",
		meta=(AllowPrivateAccess = "true"))
	int32 BaseMappingPriority = 1;

	// INPUT ACTIONS
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Controls|Input Actions",
		meta=(AllowPrivateAccess = "true"))
	UInputAction* MovementAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Controls|Input Actions",
		meta=(AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Controls|Input Actions",
		meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Controls|Input Actions",
		meta=(AllowPrivateAccess = "true"))
	UInputAction* FireAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Controls|Input Actions",
		meta=(AllowPrivateAccess = "true"))
	UInputAction* AimAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Controls|Input Actions",
		meta=(AllowPrivateAccess = "true"))
	UInputAction* SprintAction;

#pragma endregion

public:
	// Sets default values for this character's properties
	AFirst_Person_Player();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

#pragma region CAMERA

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	class UCameraComponent* CameraComp;

	// UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	// class UArrowComponent* CameraRightShoulderLocation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UArrowComponent* CameraLeftShoulderLocation;

#pragma endregion

	// INPUT FUNCTIONS
	void HandleMovement(const FInputActionInstance& Instance);

	void HandleLook(const FInputActionInstance& Instance);

	void HandleJump(const FInputActionValue& value);

	void HandleFire(const FInputActionValue& value);

	void HandleAim(const FInputActionValue& value);

	void HandleSprint(const FInputActionValue& value);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return SpringArmComp; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return CameraComp; }
};
