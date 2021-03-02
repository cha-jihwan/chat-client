// Fill out your copyright notice in the Description page of Project Settings.


#include "Alarm.h"

// Sets default values
AAlarm::AAlarm() : accumulatedTime{}
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AAlarm::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AAlarm::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	accumulatedTime += DeltaTime;
	if (1.0f < accumulatedTime)
	{

	}
}

