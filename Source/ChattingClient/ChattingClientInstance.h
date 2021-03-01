// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ChattingClient.h"
#include "Kismet/BlueprintPlatformLibrary.h"
#include "NetworkManager.h"
#include "ChattingClientInstance.generated.h"

/**
 * 
 */

//class NetworkManager;
class ULoginUIWidget;

UCLASS()
class CHATTINGCLIENT_API UChattingClientInstance : public UPlatformGameInstance
{
	GENERATED_BODY()

public:	
	UChattingClientInstance();
	bool Initialize();
	void Finalize();


	static NetworkManager* GetNetManager();
	static size_t RequestSending(char* buf, size_t size);
	static void ChangeLevel(const FString& fstr);
	static ULoginUIWidget*			GetLobby();
	static ULoginUIWidget*			lobby;
	static bool						initialized ;

private:
	static NetworkManager			netManager;
	static UChattingClientInstance* instance;
	static bool						inited;
};

