// Fill out your copyright notice in the Description page of Project Settings.


#include "ChattingLevel.h"

UChattingLevel*		UChattingLevel::level{};


// Sets default values
void UChattingLevel::NativeConstruct()
{
	Super::NativeConstruct();
	
	state = E_LevelState::ELS_NONE;

	ABLOG(Warning, "void UChattingLobby::NativeConstruct() %p", (void*)this);
}


UChattingLevel* UChattingLevel::GetLevel()
{
	return level;
}

void UChattingLevel::SetLevel(UChattingLevel* newLevel)
{
	UChattingLevel::level = newLevel;
}


E_LevelState UChattingLevel::GetState()
{
	return state;
}
void UChattingLevel::SetState(E_LevelState newLevelState)
{
	state = newLevelState;
}