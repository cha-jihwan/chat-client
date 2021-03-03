// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyUpdateHelper.h"
#include "NetworkManager.h"
#include "LoginUIWidget.h"
#include "ChattingClientInstance.h"

// Sets default values

ALobbyUpdateHelper::ALobbyUpdateHelper() : accmulatedTime{}
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ALobbyUpdateHelper::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ALobbyUpdateHelper::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	accmulatedTime += DeltaTime;
	if (1.0f <= accmulatedTime)
	{
		ULoginUIWidget::SelectRoomList();
		ULoginUIWidget::SelectUserList();

		accmulatedTime = 0.0f;
	}
}

