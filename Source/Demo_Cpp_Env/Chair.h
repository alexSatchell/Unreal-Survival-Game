// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Chair.generated.h"

UCLASS()
class DEMO_CPP_ENV_API AChair : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AChair();
	UPROPERTY(EditAnywhere, Category="Test", BlueprintReadWrite);
	int32 TotalDamage;

	UPROPERTY(EditAnywhere, Category="Test", BlueprintReadWrite);
	float DamageTimeInSeconds;

	UPROPERTY(BlueprintReadOnly, Category="Test", VisibleAnywhere, Transient);
	float DamagePerSecond;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void PostInitProperties() override;

	UFUNCTION(BlueprintCallable, Category="Test")
	void CalculateValues();

	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override; }; 