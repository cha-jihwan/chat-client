// Fill out your copyright notice in the Description page of Project Settings.


#include "ChattingLevelRoom.h"



void UChattingLevelRoom::NativeConstruct()
{
	Super::NativeConstruct();

	state = E_LevelState::ELS_ROOM;
	UChattingLevel::level = this;

	ABLOG(Warning, "void UChattingLobby::NativeConstruct() %p", (void*)this);
}