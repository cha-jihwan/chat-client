// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "GameFramework/Actor.h"
#include "ChattingClient.h"
#include "Sockets.h"
#include "Networking/Public/Networking.h"

#include <string>
#include <vector>
#include <unordered_map>

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


enum ENetUserState
{
	ENUS_None,
	ENUS_Login,
	ENUS_Lobby,
	ENUS_Room,
	ENUS_MAX
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

	void InitPacketHandler();

	/// \r\n 단위로 끊어서 리턴 없다면 ""빈 string 리턴 버퍼에서 긁음.
	bool ReadCmdLineIfHasCRLF(std::wstring& fstr);


	/// \r\n 단위로 끊어서 리턴 없다면 ""빈 string 리턴 버퍼에서 긁음.
	bool PeekCmdLineIfHasLine(std::wstring& outStr, size_t& readSize);
	void MoveReadHeadAfterPeek(size_t readSize);


	ENetUserState GetUserState();
	void SetUserState(ENetUserState state);

	/*
	*/
	static void LoginPacketHandler(const std::wstring& cmd_w);
	static void ChatPacketHandler(const std::wstring& cmd_w);
	static void WhisperPacketHandler(const std::wstring& cmd_w);
	static void CreateRoomPacketHandler(const std::wstring& cmd_w);
	static void EnterRoomPacketHandler(const std::wstring& cmd_w);
	static void LeaveRoomPacketHandler(const std::wstring& cmd_w);
	static void SelectRoomListPacketHandler(const std::wstring& cmd_w);
	static void SelectUserListInRoomPacketHandler(const std::wstring& cmd_w);
	static void SelectUserListPacketHandler(const std::wstring& cmd_w);

private:
	using PacketHandlerMap = std::unordered_map<std::wstring, void(*)(const std::wstring&)>;

	ENetManagerState		netState;
	FSocket*				sock;
	payload_buffer<65536>	sendBuffer;
	payload_buffer<65536>	recvBuffer;
	PacketHandlerMap		packetHandlerMap;
	ENetUserState			userState;
};
