// Fill out your copyright notice in the Description page of Project Settings.


#include "ChattingClientInstance.h"
//#include "NetworkManager.h"
#include "LoginUIWidget.h"


NetworkManager*				UChattingClientInstance::netManager			{};
UChattingClientInstance*	UChattingClientInstance::instance			{};
bool						UChattingClientInstance::inited				{};
bool						UChattingClientInstance::initialized		{};
UChattingRoom*				UChattingClientInstance::room				{};
UChattingLobby*				UChattingClientInstance::lobby				{};

UChattingClientInstance::UChattingClientInstance()
{
	UChattingClientInstance::instance = this;
	
	if (inited == false)
	{
		Initialize();
		inited = true;
	}
}

bool UChattingClientInstance::Initialize()
{
	netManager = new NetworkManager();
	
	return false;
}

void UChattingClientInstance::Finalize()
{
	delete netManager;
	netManager = nullptr;
}

NetworkManager* UChattingClientInstance::GetNetManager()
{
	return netManager;
}

size_t UChattingClientInstance::RequestSending(char* buf, size_t size)
{
	return netManager->PreSend(buf, size);
}

void UChattingClientInstance::ChangeLevel(const FString& levelName)
{
	UGameplayStatics::OpenLevel(instance, *levelName);
}

UChattingLobby* UChattingClientInstance::GetLobby()
{
	if (initialized == true)
	{
		ABLOG(Warning, "Lobby");
		return lobby;
	}

	return nullptr;
}

UChattingRoom* UChattingClientInstance::GetRoom()
{
	if (initialized == true)
	{
		ABLOG(Warning, "Room");
		return room;
	}

	return nullptr;
}


