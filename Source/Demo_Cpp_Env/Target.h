// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Target.generated.h"

extern FTimerHandle ChangeDirectionTimerHandler;

UCLASS()
class DEMO_CPP_ENV_API ATarget : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATarget();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float m_Speed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float m_TimeToChange;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector m_Direction;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float deltaTime) override;

	void Move(float DeltaTime);

	UFUNCTION(BlueprintCallable, Category="Events")
	void ChangeDirection();
};
