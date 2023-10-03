// Fill out your copyright notice in the Description page of Project Settings.
#include "Target.h"

FTimerHandle ChangeDirectionTimerHandler;

// Sets default values
ATarget::ATarget() : m_Speed(200.0f), m_TimeToChange(3.0f), m_Direction(0.0f,1.0f, 0.0f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATarget::BeginPlay()
{
	Super::BeginPlay();

	// Change target direction based on timer
	GetWorldTimerManager().SetTimer(ChangeDirectionTimerHandler, this, &ATarget::ChangeDirection, m_TimeToChange, true);
}

// Called every frame
void ATarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Move(DeltaTime);
}

void ATarget::Move(float deltaTime)
{
	// Normalize vector
	m_Direction.Normalize();

	// Get relative speed
	const float RelativeSpeed = m_Speed * deltaTime;

	// Update Location
	const FVector DeltaLocation = m_Direction * (float)RelativeSpeed;
	this->AddActorWorldOffset(DeltaLocation);
}

void ATarget::ChangeDirection()
{
	m_Direction = m_Direction * -1.0f;
}


