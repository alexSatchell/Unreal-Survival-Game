// Fill out your copyright notice in the Description page of Project Settings.
#include "Chair.h"

// Sets default values
AChair::AChair() : TotalDamage(200), DamageTimeInSeconds(1.0f)
{
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AChair::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AChair::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	const FVector WorldOffset = FVector(100.0f, 0.0f, 0.0f);
	this->AddActorWorldOffset(WorldOffset * DeltaTime, true);
}

// Called to bind functionality to input
void AChair::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AChair::PostInitProperties()
{
	Super::PostInitProperties();
}

void AChair::CalculateValues()
{
	DamagePerSecond = TotalDamage / DamageTimeInSeconds;
}

// This code will be auto removed, it is wrapped in an editor-specific #ifdef
void AChair::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	CalculateValues();

	Super::PostEditChangeProperty(PropertyChangedEvent);
}





