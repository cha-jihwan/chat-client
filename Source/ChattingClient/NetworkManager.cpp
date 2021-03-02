// Fill out your copyright notice in the Description page of Project Settings.



//#include "ChttingClient.h"
#include "NetworkManager.h"
#include "ContentTexts.h"
#include "ChattingClientGameMode.h"
#include "ChattingClientInstance.h"
#include "LoginUIWidget.h"
//#include <codecvt>
#include <stdlib.h>
#include <clocale>

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
	ABLOG(Warning, "ENMS_Connecting");

	InitPacketHandler();

	return true;
}

bool NetworkManager::Disconnect()
{
	if (nullptr == sock)
	{
		ABLOG(Warning, "socket already disconncted....");

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
			//ABLOG(Warning, "ENMS_Initialized"));
			break;

		case ENMS_Connecting: // 연결 중..
			TryConnect();
			break;

		case ENMS_Connected: // 연결 이후...
			TryRecvPayload();
			TrySendPayload();
			break;

		case ENMS_Disconnecting: // 
			//ABLOG(Warning, "ENMS_Disconnecting"));
			break;
		case ENMS_Disconnected:
			//ABLOG(Warning, "ENMS_Disconnected"));
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
		ABLOG(Warning, "sentSize != ToSendSize");
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
			ABLOG(Warning, "char text : %s %d", recvBuffer.get_write_buffer() - receivedBytes, receivedBytes);
	
			if (nullptr != UChattingClientInstance::GetLobby())
			{
				UChattingClientInstance::GetLobby()->AddChatMsgInLobby(L"z한z");
			}

			ParsePayload();
		}

		if (0 == recvBuffer.get_buffer_size())
		{
			ABLOG(Warning, "RecvBuffer Full...");
		}

		return receivedBytes;
	}

	state = ENMS_Disconnected;
	ABLOG(Warning, "ENMS_Disconnected");

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
		ABLOG(Warning, "ENMS_Connected");
		
		return true;
	}

	ABLOG(Warning, "TryConnect return false;");
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
			ABLOG(Warning, "fail");
		}
	}
	
	return sentBytes;
}

void NetworkManager::InitPacketHandler()
{
	packetHandlerMap.emplace(PacketFilterKeywords[(size_t)e_packet_filter::EPF_LOGIN_OK], NetworkManager::LoginPacketHandler);
	packetHandlerMap.emplace(PacketFilterKeywords[(size_t)e_packet_filter::EPF_ROOM_CREATING_OK], NetworkManager::CreateRoomPacketHandler);
	packetHandlerMap.emplace(PacketFilterKeywords[(size_t)e_packet_filter::EPF_ROOM_EXITING_OK], NetworkManager::LeaveRoomPacketHandler);
	packetHandlerMap.emplace(PacketFilterKeywords[(size_t)e_packet_filter::EPF_ROOM_ENTERING_OK], NetworkManager::EnterRoomPacketHandler);
	packetHandlerMap.emplace(PacketFilterKeywords[(size_t)e_packet_filter::EPF_CHAT], NetworkManager::ChatPacketHandler);
	packetHandlerMap.emplace(PacketFilterKeywords[(size_t)e_packet_filter::EPF_WHISPER], NetworkManager::WhisperPacketHandler);
	packetHandlerMap.emplace(PacketFilterKeywords[(size_t)e_packet_filter::EPF_SELECT_ROOM_LIST], NetworkManager::SelectRoomListPacketHandler);
	packetHandlerMap.emplace(PacketFilterKeywords[(size_t)e_packet_filter::EPF_SELECT_USER_LIST], NetworkManager::SelectUserListPacketHandler);
	packetHandlerMap.emplace(PacketFilterKeywords[(size_t)e_packet_filter::EPF_SELECT_USER_LIST_IN_ROOM], NetworkManager::SelectUserListInRoomPacketHandler);
}



void NetworkManager::LoginPacketHandler(const std::wstring& cmd_w)
{
	ABLOG(Warning, "LoginPacketHandler");
	FString fstr1 = L"LevelLobby";
	UChattingClientInstance::ChangeLevel(fstr1);
}

void NetworkManager::ChatPacketHandler(const std::wstring& cmd_w)
{
	FString fst(cmd_w.c_str(), cmd_w.size());

	UChattingClientInstance::lobby->AddChatMsgInLobby(fst);
}
	
void NetworkManager::WhisperPacketHandler(const std::wstring& cmd_w)
{
	FString fstr(cmd_w.c_str(), cmd_w.size());

	UChattingClientInstance::lobby->AddChatMsgInLobby(fstr);
}

void NetworkManager::CreateRoomPacketHandler(const std::wstring& cmd_w)
{
	FString roomLevel = L"LevelRoom";

	UChattingClientInstance::ChangeLevel(roomLevel);
}

void NetworkManager::EnterRoomPacketHandler(const std::wstring& cmd_w)
{
	FString roomLevel = L"LevelRoom";

	UChattingClientInstance::ChangeLevel(roomLevel);
}

void NetworkManager::LeaveRoomPacketHandler(const std::wstring& cmd_w)
{
	FString lobbyLevel = L"LevelLobby";

	UChattingClientInstance::ChangeLevel(lobbyLevel);
}

void NetworkManager::SelectRoomListPacketHandler(const std::wstring& cmd_w)
{
	std::wstring nextLine;
	for(;;)
	{
		size_t readSize{};
		nextLine.clear();
		if (false == UChattingClientInstance::GetNetManager()->PeekCmdLineIfHasLine(nextLine, readSize))
		{
			return;
		}

		UChattingClientInstance::GetNetManager()->MoveReadHead(readSize);

		ABLOG(Warning, "방 이름 : %ws", nextLine.c_str());
		size_t findPos = nextLine.find(L"번 방 : ");
		if (std::wstring::npos == findPos)
		{
			return;
		}
		ABLOG(Warning, "방 찾음 : %ws", nextLine.c_str());


		int32 nextLineLength = nextLine.size();
		FString fst{ nextLine.c_str(), nextLineLength };

		UChattingClientInstance::lobby->AddRoomInfoInLobby(fst);
	}

}

	
	

void NetworkManager::SelectUserListInRoomPacketHandler(const std::wstring& cmd_w)
{

}

void NetworkManager::SelectUserListPacketHandler(const std::wstring& cmd_w)
{

}

bool NetworkManager::ReadCmdLineIfHasCRLF(std::wstring& inStr)
{
	constexpr int CRLF_SIZE = 2;
	char* b = recvBuffer.get_header();
	
	std::string buf(recvBuffer.get_header(), recvBuffer.get_use_size());
	size_t retPos = buf.find("\r\n");
	if (std::string::npos == retPos) // 문장가 없다면 ㅂㅂ~
	{
		return false;
	}

	std::string cmd(recvBuffer.get_header(), retPos);

	recvBuffer.move_read_head(retPos + CRLF_SIZE);

	TCHAR buffer[1024]{};
	size_t wstr_size = mbstowcs(buffer, cmd.c_str(), 1024);

	std::wstring cmd_w{ buffer , wstr_size };
	inStr = cmd_w;


	return true;
}

bool NetworkManager::PeekCmdLineIfHasLine(std::wstring& outStr, size_t& readSize)
{
	constexpr int CRLF_SIZE = 2;
	char* b = recvBuffer.get_header();

	std::string buf(recvBuffer.get_header(), recvBuffer.get_use_size());
	size_t retPos = buf.find("\r\n");
	if (std::string::npos == retPos) // 문장가 없다면 ㅂㅂ~
	{
		return false;
	}

	std::string cmd(recvBuffer.get_header(), retPos);

	readSize = retPos + CRLF_SIZE;

	TCHAR buffer[1024]{};
	size_t wstr_size = mbstowcs(buffer, cmd.c_str(), 1024);
	
	std::wstring cmd_w{ buffer , wstr_size };
	outStr = std::move(cmd_w);

	return true;
}

void NetworkManager::MoveReadHead(size_t readSize)
{
	recvBuffer.move_read_head(readSize);
}

void NetworkManager::ParsePayload()
{
	std::wstring cmd_w;
	for (;;)
	{
		cmd_w.clear();
		// "\r\n" 단위로 끊어서 wstring 얻어오기.
		if (false == ReadCmdLineIfHasCRLF(cmd_w))
		{
			return;
		}

		ABLOG(Warning, "%ws", cmd_w.c_str());

		// 패킷 필터링...
		for (const auto& keyword : PacketFilterKeywords)
		{
			// 해당하는 경우...

			size_t pos = cmd_w.find(keyword);
			if (std::wstring::npos != pos) 
			{
				packetHandlerMap[keyword](cmd_w);
			}
		}
	}


	recvBuffer.rewind();
}

