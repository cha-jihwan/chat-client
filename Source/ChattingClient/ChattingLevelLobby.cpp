// Fill out your copyright notice in the Description page of Project Settings.


#include "ChattingLevelLobby.h"

// Sets default values
void UChattingLevelLobby::NativeConstruct()
{
	Super::NativeConstruct();

	state = E_LevelState::ELS_LOBBY;
	UChattingLevel::level = this;

	ABLOG(Warning, "void UChattingLobby::NativeConstruct() %p", (void*)this);
}