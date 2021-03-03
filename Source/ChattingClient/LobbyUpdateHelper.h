// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ChattingClient.h"
#include "GameFramework/Actor.h"
#include "LobbyUpdateHelper.generated.h"

UCLASS()
class CHATTINGCLIENT_API ALobbyUpdateHelper : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALobbyUpdateHelper();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	float accmulatedTime;
};
