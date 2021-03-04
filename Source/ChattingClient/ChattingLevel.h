// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ChattingClient.h"
#include "Blueprint/UserWidget.h"
#include "ChattingLevel.generated.h"

/**
 * 
 */

enum class E_LevelState
{
	ELS_NONE,
	ELS_LOGIN,
	ELS_LOBBY,
	ELS_ROOM,
	ELS_MAX,
};

UCLASS(blueprintable)
class CHATTINGCLIENT_API UChattingLevel : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	//virtual void NativeDestruct() override;

public:
	// for all
	// for lobby
	UFUNCTION(BlueprintImplementableEvent, Category = "UMG")
	void AddChatMsgInLobby(const FString& fst, bool isMine);

	UFUNCTION(BlueprintImplementableEvent, Category = "UMG")
	void AddRoomInfoInLobby(const FString& fst);

	UFUNCTION(BlueprintImplementableEvent, Category = "UMG")
	void AddUserInfoInLobby(const FString& fst);

	UFUNCTION(BlueprintImplementableEvent, Category = "UMG")
	void ClearUserInfoListInLobby();

	UFUNCTION(BlueprintImplementableEvent, Category = "UMG")
	void ClearRoomInfoList();
	
	// for room
	UFUNCTION(BlueprintImplementableEvent, Category = "UMG")
	void AddChatMsgInRoom(const FString& fst, bool isMine);

	UFUNCTION(BlueprintImplementableEvent, Category = "UMG")
	void AddUserInfoInRoom(const FString& fst);

	UFUNCTION(BlueprintImplementableEvent, Category = "UMG")
	void ClearUserInfoListInRoom();

	// for login
	E_LevelState GetState();
	void SetState(E_LevelState newLevelState);

	static UChattingLevel* GetLevel();
	static void SetLevel(UChattingLevel* newLevel);

protected:
	static UChattingLevel*	level;
	E_LevelState			state;
};

