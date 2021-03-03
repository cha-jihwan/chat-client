// Copyright Epic Games, Inc. All Rights Reserved.

#include "ChattingClientGameMode.h"
#include "ChattingClientInstance.h"
#include "NetworkManager.h"
#include "UObject/ConstructorHelpers.h"

#include "LoginUIWidget.h"
#include "Blueprint/WidgetTree.h"


AChattingClientGameMode::AChattingClientGameMode()
	: Super()
{
	PrimaryActorTick.bCanEverTick = true;
	// use our custom HUD class
	//HUDClass = AChattingClientHUD::StaticClass();
}

void AChattingClientGameMode::BeginPlay()
{
	Super::BeginPlay();

	//UChattingClientInstance* ins = (UChattingClientInstance*)GetGameInstance();
	//ins->Initialize();

	//ChangeMenuWidget(ULoginUIWidget::StaticClass());
}

//void AChattingClientGameMode::EndPlay()
//{
//	Super::EndPlay();
//
//
//	//ChangeMenuWidget(ULoginUIWidget::StaticClass());
//}

void AChattingClientGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UChattingClientInstance* ins = (UChattingClientInstance*)GetGameInstance();
	ins->GetNetManager()->OnTick(DeltaTime);
}

void AChattingClientGameMode::ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass)
{
	if (CurrentWidget != nullptr)
	{
		CurrentWidget->RemoveFromViewport();  // Before widget delete
		CurrentWidget = nullptr;
	}
	if (NewWidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), NewWidgetClass);
		if (CurrentWidget != nullptr)
		{
			//CurrentWidget->Size
			CurrentWidget->AddToViewport(); // View current widget
		}
	}
}