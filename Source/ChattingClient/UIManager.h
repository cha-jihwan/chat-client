// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ChattingClient.h"
#include "GameFramework/Actor.h"
#include "UIManager.generated.h"

UCLASS(Blueprintable)
class CHATTINGCLIENT_API AUIManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AUIManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent)
		void bp_test();
};
