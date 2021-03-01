// Fill out your copyright notice in the Description page of Project Settings.



//#include "ChttingClient.h"
#include "NetworkManager.h"
#include "ChattingClientGameMode.h"
#include "ChattingClientInstance.h"
#include "LoginUIWidget.h"
//#include <codecvt>
#include <stdlib.h>
#include <clocale>
#include <string>
#include <unordered_map>
#define _CRT_SECURE_NO_WARNINGS

// Sets default values
NetworkManager::NetworkManager() 
	: sock{ nullptr }, state{ ENMS_Initialized }//, sendBuffer{}, recvBuffer{}
{
	Initialize();
}

bool NetworkManager::Initialize()
{
	std::setlocale(LC_ALL, "Korean");

	sock = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("default"), false);
	sock->SetNonBlocking(true);

	state = ENMS_Connecting;
	ABLOG(Warning, TEXT("ENMS_Connecting"));


	//packetHandlerMap.emplace("");

	return true;
}

bool NetworkManager::Disconnect()
{
	if (nullptr == sock)
	{
		ABLOG(Warning, TEXT("socket already disconncted...."));

		return false;
	}

	sock->Shutdown(ESocketShutdownMode::Read);
	sock = nullptr;
	state = ENMS_Disconnected;

	return true;
}

// Called when the game starts or when spawned
void NetworkManager::OnBeginPlay() { }

// Called every frame
void NetworkManager::OnTick(float DeltaTime)
{
	switch (state)
	{
		case ENMS_Initialized: // 연결 시도 전..
			//ABLOG(Warning, TEXT("ENMS_Initialized"));
			break;

		case ENMS_Connecting: // 연결 중..
			TryConnect();
			break;

		case ENMS_Connected: // 연결 이후...
			TryRecvPayload();
			TrySendPayload();
			break;

		case ENMS_Disconnecting: // 
			//ABLOG(Warning, TEXT("ENMS_Disconnecting"));
			break;
		case ENMS_Disconnected:
			//ABLOG(Warning, TEXT("ENMS_Disconnected"));
			break;

		default:
			break;
	}
}

int64 NetworkManager::PreSend(char* buffer, int64 size)
{
	size_t preSentSize = sendBuffer.write(buffer, size);
	if (preSentSize != size)
	{
		ABLOG(Warning, TEXT("sentSize != ToSendSize"));
	}

	return preSentSize;
}


int64 NetworkManager::TryRecvPayload()
{
	int32 receivedBytes = 0;
	
	bool isSuccess = sock->Recv((uint8*)recvBuffer.get_write_buffer(), recvBuffer.get_buffer_size(), receivedBytes);
	recvBuffer.move_write_head(receivedBytes);

	if (isSuccess) // nonblocking 소켓이라 0바이트 받아도 성공
	{

		if (0 < receivedBytes)
		{
			ABLOG(Warning, TEXT("char text : %s %d"), recvBuffer.get_write_buffer() - receivedBytes, receivedBytes);
	
			if (nullptr != UChattingClientInstance::GetLobby())
			{
				UChattingClientInstance::GetLobby()->AddChatMsg(L"z한z");
			}

			ParsePayload();
		}

		if (0 == recvBuffer.get_buffer_size())
		{
			ABLOG(Warning, TEXT("RecvBuffer Full..."));
		}

		return receivedBytes;
	}

	state = ENMS_Disconnected;
	ABLOG(Warning, TEXT("ENMS_Disconnected"));

	return 0;
}

bool NetworkManager::TryConnect()
{
	FIPv4Address ip(127, 0, 0, 1);
	TSharedRef<FInternetAddr> addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	addr->SetIp(ip.Value);
	addr->SetPort(5500);

	if (ENMS_Connecting < state)
	{
		return false; // already disconneted.... or connected.....
	}
	if (ENMS_Connecting > state)
	{
		state = ENMS_Connecting;
	}

	bool connected = sock->Connect(*addr);
	if (connected)
	{
		state = ENMS_Connected;
		ABLOG(Warning, TEXT("ENMS_Connected"));
		
		return true;
	}

	ABLOG(Warning, TEXT("TryConnect return false;"));
	return false;
}


int64 NetworkManager::TrySendPayload()
{
	int sentBytes = 0;

	if (0 < sendBuffer.get_use_size())
	{
		bool isSuccess = sock->Send((uint8*)sendBuffer.get_buffer(), sendBuffer.get_use_size(), sentBytes);
		if (isSuccess)
		{
			sendBuffer.move_read_head(sentBytes);
			sendBuffer.rewind();
		}
		else
		{
			ABLOG(Warning, TEXT("fail"));
		}
	}
	
	return sentBytes;
}


void NetworkManager::ParsePayload()
{
	constexpr int CRLF_SIZE = 2;

	//size_t parsedSize = 0;
	for (;;)
	{
		std::string buf(recvBuffer.get_header(), recvBuffer.get_use_size());
		size_t retPos = buf.find("\r\n");
		if (std::string::npos == retPos) // 문장가 없다면 ㅂㅂ~
		{
			break;
		}

		std::string cmd(recvBuffer.get_header(), retPos);

		ABLOG(Warning, TEXT("%s"), cmd.c_str());

		recvBuffer.move_read_head(retPos + CRLF_SIZE);
		
		TCHAR buffer[1024]{};
		size_t wstr_size = mbstowcs(buffer, cmd.c_str(), 1024);
		std::wstring cmd_w{ buffer , wstr_size};
		//cmd_w.assign(cmd.begin(), cmd.end());


		if (std::string::npos != cmd_w.find(L"로그인 성공"))
		{
			FString fstr1 = L"LevelLobby";
			UChattingClientInstance::ChangeLevel(fstr1);
		}
	}

	recvBuffer.rewind();
}

