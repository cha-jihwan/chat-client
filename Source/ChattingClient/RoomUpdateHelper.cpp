// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomUpdateHelper.h"
#include "NetworkManager.h"
#include "LoginUIWidget.h"
#include "ChattingClientInstance.h"

// Sets default values
ARoomUpdateHelper::ARoomUpdateHelper() : accmulatedTime{}
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARoomUpdateHelper::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ARoomUpdateHelper::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	accmulatedTime += DeltaTime;
	if (1.0f <= accmulatedTime)
	{
		ULoginUIWidget::SelectUserListInRoom();

		accmulatedTime = 0.0f;
	}
}

