// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "GameFramework/Actor.h"
#include "ChattingClient.h"
#include "Sockets.h"
#include "Networking/Public/Networking.h"
#include "../ChattingClient/payload_ buffer.h"
//#include "NetworkManager.generated.h"


enum ENetManagerState
{
	ENMS_Initialized,
	ENMS_Connecting,
	ENMS_Connected,
	ENMS_Disconnecting,
	ENMS_Disconnected,
	ENMS_MAX
};

//UCLASS()
class NetworkManager
//class CHATTINGCLIENT_API ANetworkManager : public AActor
{
public:	
	// Sets default values for this actor's properties
	NetworkManager();

	// Called every frame
	//virtual void Tick(float DeltaTime) override;
	void OnTick(float DeltaTime);

	int64 PreSend(char* buffer, int64 size);
	
	// Called when the game starts or when spawned
	//virtual void BeginPlay() override;
	void OnBeginPlay();

	int64 TryRecvPayload();
	int64 TrySendPayload();
	void ParsePayload();

	bool TryConnect();

	bool Initialize();
	bool Disconnect();

	ENetManagerState		state;
	FSocket*				sock;
	payload_buffer<65536>	sendBuffer;
	payload_buffer<65536>	recvBuffer;
	//std::unordered_map<std::string, size_t(*)()> packetHandlerMap;
};
