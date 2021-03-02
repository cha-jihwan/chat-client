// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ChattingClient.h"
#include "Kismet/BlueprintPlatformLibrary.h"
#include "NetworkManager.h"
#include "ChattingLobby.h"
#include "ChattingClientInstance.generated.h"

/**
 * 
 */

class UChattingLobby;
class ULoginUIWidget;

UCLASS()
class CHATTINGCLIENT_API UChattingClientInstance : public UPlatformGameInstance
{
	GENERATED_BODY()

public:	
	UChattingClientInstance();
	bool Initialize();
	void Finalize();


	static NetworkManager*			GetNetManager();
	static size_t					RequestSending(char* buf, size_t size);
	static void						ChangeLevel(const FString& fstr);
	static UChattingLobby*			GetLobby();
	static UChattingLobby*			lobby;
	static bool						initialized;

private:
	static NetworkManager*				netManager;
	static UChattingClientInstance*		instance;
	static bool							inited;
};

