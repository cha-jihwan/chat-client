// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ChattingClient.h"
#include "GameFramework/GameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "ChattingClientGameMode.generated.h"

UCLASS(minimalapi)
class AChattingClientGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AChattingClientGameMode();
	
	UFUNCTION(BlueprintCallable, Category = "UMG Game")
	void ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass); // Widget change function


	//virtual void Tick(float DeltaTime) override final;
protected:
	virtual void BeginPlay() override final;
	//virtual void EndPlay() override final;
	virtual void Tick(float DeltaTime) override final;



	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG Game")
	TSubclassOf<UUserWidget> StartingWidgetClass; // Main widget

	UPROPERTY()
	UUserWidget* CurrentWidget; // Output widget

};


