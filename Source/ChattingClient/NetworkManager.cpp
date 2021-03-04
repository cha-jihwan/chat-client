// Fill out your copyright notice in the Description page of Project Settings.



//#include "ChttingClient.h"
#include "NetworkManager.h"
#include "ContentTexts.h"
#include "ChattingClientGameMode.h"
#include "ChattingClientInstance.h"
#include "ChattingLevel.h"
#include "LoginUIWidget.h"
//#include <codecvt>
#include <stdlib.h>
#include <clocale>

#define _CRT_SECURE_NO_WARNINGS

// Sets default values
NetworkManager::NetworkManager() 
	: sock{ nullptr }
	, netState{ ENMS_Initialized }, userState{ ENUS_None }//, sendBuffer{}, recvBuffer{}
{
	Initialize();
}

bool NetworkManager::Initialize()
{
	std::setlocale(LC_ALL, "Korean");

	sock = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("default"), false);
	if (nullptr == sock)
	{
		ABLOG(Error, "Socket Creation Failure....");
		return false;
	}

	sock->SetNonBlocking(true);

	netState = ENMS_Connecting;
	userState = ENUS_Login;

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

	sock->Shutdown(ESocketShutdownMode::Write);
	
	sock->Close();

	sock = nullptr;

	netState = ENMS_Disconnected;

	return true;
}



// Called when the game starts or when spawned
void NetworkManager::OnBeginPlay() { }

// Called every frame
void NetworkManager::OnTick(float DeltaTime)
{
	switch (netState)
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
			ABLOG(Warning, "말도 안되는... 일이 일어남..");
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
	if (nullptr == sock)
	{
		ABLOG(Warning, "socket already disconncted....");
		return 0;
	}

	int32 receivedBytes = 0;
	
	bool isSuccess = sock->Recv((uint8*)recvBuffer.get_write_buffer(), recvBuffer.get_buffer_size(), receivedBytes);
	recvBuffer.move_write_head(receivedBytes);

	if (isSuccess) // nonblocking 소켓이라 0바이트 받아도 성공
	{
		if (0 < receivedBytes)
		{
			ABLOG(Warning, "recved bytes : %d", receivedBytes);
	
			//if (nullptr != UChattingLevel::GetLevel())
			//{
			//	UChattingLevel::GetLevel()->AddChatMsgInLobby(L"z한z", true);
			//}

			ParsePayload();
		}

		if (0 == recvBuffer.get_buffer_size())
		{
			ABLOG(Warning, "RecvBuffer Full...");
		}

		return receivedBytes;
	}

	netState = ENMS_Disconnected;
	ABLOG(Warning, "ENMS_Disconnected");

	return 0;
}

bool NetworkManager::TryConnect()
{
	if (nullptr == sock)
	{
		ABLOG(Warning, "socket already disconncted....");
		return 0;
	}


	FIPv4Address IPAddr;
	FIPv4Address::Parse(TEXT("127.0.0.1"), IPAddr);

	TSharedRef<FInternetAddr> addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	addr->SetIp(IPAddr.Value);
	addr->SetPort(5500);

	if (ENMS_Connecting < netState)
	{
		return false; // already disconneted.... or connected.....
	}
	if (ENMS_Connecting > netState)
	{
		netState = ENMS_Connecting;
	}

	bool connected = sock->Connect(*addr);
	if (connected)
	{
		// 논 블로킹 소켓 커넥션 확인 방법.
		// reference : https://answers.unrealengine.com/questions/137371/how-can-i-check-socket-state.html
		if (ESocketConnectionState::SCS_Connected == sock->GetConnectionState())
		{
			netState = ENMS_Connected;

			ABLOG(Warning, "ENMS_Connected...");
		}
		else
		{
			ABLOG(Warning, "ENMS_PendingConnection...");
		}

		return true;
	}
	else // 연결중... 등 
	{
		if (ESocketConnectionState::SCS_Connected == sock->GetConnectionState())
		{
			netState = ENMS_Connected;

			ABLOG(Warning, "ENMS_Connected...");
		}
		else
		{
			ABLOG(Warning, "TryConnect return false;");
		}

		return false;
	}
}


int64 NetworkManager::TrySendPayload()
{
	if (nullptr == sock)
	{
		ABLOG(Warning, "socket already disconncted....");
		return 0;
	}


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
	packetHandlerMap.emplace(PacketFilterKeywords[(size_t)e_packet_filter::EPF_WHISPER], NetworkManager::ChatPacketHandler);
	packetHandlerMap.emplace(PacketFilterKeywords[(size_t)e_packet_filter::EPF_SELECT_ROOM_LIST], NetworkManager::SelectRoomListPacketHandler);
	packetHandlerMap.emplace(PacketFilterKeywords[(size_t)e_packet_filter::EPF_SELECT_USER_LIST], NetworkManager::SelectUserListPacketHandler);
	packetHandlerMap.emplace(PacketFilterKeywords[(size_t)e_packet_filter::EPF_SELECT_USER_LIST_IN_ROOM], NetworkManager::SelectUserListInRoomPacketHandler);
}



void NetworkManager::LoginPacketHandler(const std::wstring& cmdW)
{
	ABLOG(Warning, "LoginPacketHandler");
	
	size_t findPos = cmdW.find(TEXT("님"));
	if(std::wstring::npos == findPos)
	{
		ABLOG_S(Error);
	}

	std::wstring newName = cmdW.substr(0, findPos);

	UChattingClientInstance::GetNetManager()->SetUserName(newName);

	FString fstr = L"LevelLobby";

	UChattingClientInstance::ChangeLevel(fstr);


	ABLOG_S(Warning);
	//UChattingClientInstance::GetNetManager()->SetUserState(ENUS_Lobby);
}

void NetworkManager::ChatPacketHandler(const std::wstring& cmdW)
{
	ABLOG_S(Warning);

	FString fst(cmdW.c_str(), cmdW.size());

	bool isMine = false;
	size_t findPos = cmdW.find(UChattingClientInstance::GetNetManager()->GetUserName());
	if (std::wstring::npos != findPos)
	{
		isMine = true;
	}

	//bool isMine = std::wstring::npos != cmdW.find(UChattingClientInstance::GetNetManager()->GetName());

	switch (UChattingLevel::GetLevel()->GetState())
	{
	case E_LevelState::ELS_LOBBY:
		UChattingLevel::GetLevel()->AddChatMsgInLobby(fst, isMine);
		break;
	case E_LevelState::ELS_ROOM:
		UChattingLevel::GetLevel()->AddChatMsgInRoom(fst, isMine);
		break;

	default:
		ABLOG(Warning, "NetUserState %d  In ChatHandler", UChattingLevel::GetLevel()->GetState());
	}
}

	
void NetworkManager::WhisperPacketHandler(const std::wstring& cmdW)
{
	ABLOG_S(Warning);
	FString fstr(cmdW.c_str(), cmdW.size());

	//UChattingClientInstance::GetNetManager()->GetUserState();

	std::wstring extractedName = cmdW.substr(0, cmdW.size());

	bool isMine = extractedName != UChattingClientInstance::GetNetManager()->GetUserName();

	switch (UChattingLevel::GetLevel()->GetState())
	{
	case E_LevelState::ELS_LOBBY: 
		UChattingLevel::GetLevel()->AddChatMsgInLobby(fstr, isMine);
		break;
	case E_LevelState::ELS_ROOM:
		UChattingLevel::GetLevel()->AddChatMsgInRoom(fstr, isMine);
		break;

	default:
		ABLOG(Warning, "NetUserState %d  In Whisper", UChattingLevel::GetLevel()->GetState());
	}
}

void NetworkManager::CreateRoomPacketHandler(const std::wstring& cmdW)
{
	constexpr size_t keywordSize = (sizeof(L"번 방 : ") - sizeof(L'\0')) / 2;

	ABLOG_S(Warning);

	FString roomLevel = L"LevelRoom";

	UChattingClientInstance::ChangeLevel(roomLevel);

	//UChattingClientInstance::GetNetManager()->SetUserState(ENUS_Room);

	size_t findPos = cmdW.find(L"번 방 : ");
	std::wstring roomName = cmdW.substr(findPos + keywordSize);

	UChattingClientInstance::GetNetManager()->SetRoomName(roomName);

}

/*
텍스트 형식 :
전체 방 목록
1번 방 : ew

*/
void NetworkManager::EnterRoomPacketHandler(const std::wstring& cmdW)
{
	constexpr size_t keywordSize = (sizeof(L"번 방 : ") - sizeof(L'\0'))/2;

	ABLOG_S(Warning);

	FString roomLevel = L"LevelRoom";

	UChattingClientInstance::ChangeLevel(roomLevel);

	//UChattingClientInstance::GetNetManager()->SetUserState(ENUS_Room);

	size_t findPos = cmdW.find(L"번 방 : ");
	std::wstring roomName = cmdW.substr(findPos + keywordSize); 

	UChattingClientInstance::GetNetManager()->SetRoomName(roomName);
}

void NetworkManager::LeaveRoomPacketHandler(const std::wstring& cmdW)
{
	ABLOG_S(Warning);

	FString lobbyLevel = L"LevelLobby";

	UChattingClientInstance::ChangeLevel(lobbyLevel);
}

void NetworkManager::SelectRoomListPacketHandler(const std::wstring& cmdW)
{
	ABLOG_S(Warning);

	UChattingLevel::GetLevel()->ClearRoomInfoList();

	std::wstring nextLine;
	for(;;)
	{
		size_t readSize{};
		nextLine.clear();
		if (false == UChattingClientInstance::GetNetManager()->PeekCmdLineIfHasLine(nextLine, readSize))
		{
			return;
		}

		UChattingClientInstance::GetNetManager()->MoveReadHeadAfterPeek(readSize);

		ABLOG(Warning, "방 이름 : %ws", nextLine.c_str());
		size_t findPos = nextLine.find(L"번 방 : ");
		if (std::wstring::npos == findPos)
		{
			return;
		}


		int32 nextLineLength = nextLine.size();
		FString fst{ nextLine.c_str(), nextLineLength };

		UChattingLevel::GetLevel()->AddRoomInfoInLobby(fst);
	}

}

	
	

void NetworkManager::SelectUserListInRoomPacketHandler(const std::wstring& cmdW)
{
	ABLOG_S(Warning);

	UChattingLevel::GetLevel()->ClearUserInfoListInRoom();

	std::wstring nextLine;
	for (;;)
	{
		size_t readSize{};
		nextLine.clear();
		if (false == UChattingClientInstance::GetNetManager()->PeekCmdLineIfHasLine(nextLine, readSize))
		{
			return;
		}

		UChattingClientInstance::GetNetManager()->MoveReadHeadAfterPeek(readSize);

		ABLOG(Warning, "유저 이름 : %ws", nextLine.c_str());
		size_t findPos = nextLine.find(TEXT("님."));
		if (std::wstring::npos == findPos)
		{
			return;
		}

		ABLOG(Warning, "유저 맞음 : %ws", nextLine.c_str());

		int32 nextLineLength = nextLine.size();
		FString fst{ nextLine.c_str(), nextLineLength };

		UChattingLevel::GetLevel()->AddUserInfoInRoom(fst);
	}
}

void NetworkManager::SelectUserListPacketHandler(const std::wstring& cmdW)
{
	ABLOG(Warning, "SelectUserListPacketHandler %p", (void*)UChattingLevel::GetLevel());
	UChattingLevel::GetLevel()->ClearUserInfoListInLobby();
	ABLOG(Warning, "SelectUserListPacketHandler %p", (void*)UChattingLevel::GetLevel());

	std::wstring nextLine;
	for (;;)
	{
		size_t readSize{};
		nextLine.clear();
		if (false == UChattingClientInstance::GetNetManager()->PeekCmdLineIfHasLine(nextLine, readSize))
		{
			return;
		}

		UChattingClientInstance::GetNetManager()->MoveReadHeadAfterPeek(readSize);

		ABLOG(Warning, "유저 이름 : %ws", nextLine.c_str());
		size_t findPos = nextLine.find(TEXT("님."));
		if (std::wstring::npos == findPos)
		{
			return;
		}

		ABLOG(Warning, "유저 맞음 : %ws", nextLine.c_str());

		int32 nextLineLength = nextLine.size();
		FString fst{ nextLine.c_str(), nextLineLength };

		UChattingLevel::GetLevel()->AddUserInfoInLobby(fst);
	}

	ABLOG(Warning, "OK");
}

void NetworkManager::CahttingCommonHnadler(const std::wstring& cmdW)
{
}

bool NetworkManager::ReadCmdLineIfHasCRLF(std::wstring& outStr)
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

	wchar_t buffer[1024]{};
	size_t wstr_size = mbstowcs(buffer, cmd.c_str(), 1024);

	std::wstring cmdW{ buffer , wstr_size };
	outStr = cmdW;


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
	size_t wstrSize = mbstowcs(buffer, cmd.c_str(), 1024);
	//if (((size_t)-1) == wstrSize) // NULL 문자 존재 비워야 한다.
	//{
	//	return false;
	//}

	std::wstring cmdW {buffer , wstrSize};
	outStr = std::move(cmdW);

	return true;
}

void NetworkManager::MoveReadHeadAfterPeek(size_t readSize)
{
	recvBuffer.move_read_head(readSize);
}





void NetworkManager::SetUserName(std::wstring& newName)
{
	userName = newName;

	ABLOG(Warning, "SessionNmae : %ws", this->userName.c_str());
}

void NetworkManager::SetRoomName(std::wstring& newName)
{
	roomName = newName;

	ABLOG(Warning, "RoomNmae : %ws", this->roomName.c_str());
}

const std::wstring& NetworkManager::GetUserName()
{
	return userName;
}

const std::wstring& NetworkManager::GetRoomName()
{
	return roomName;
}

void NetworkManager::ParsePayload()
{
	std::wstring cmdW;
	for (;;)
	{
		cmdW.clear();
		// "\r\n" 단위로 끊어서 wstring 얻어오기.
		if (false == ReadCmdLineIfHasCRLF(cmdW))
		{
			return;
		}

		ABLOG(Warning, "%ws", cmdW.c_str());

		// 패킷 필터링...
		for (const auto& keyword : PacketFilterKeywords)
		{
			// 해당하는 경우...
			size_t pos = cmdW.find(keyword);
			if (std::wstring::npos != pos) 
			{
				packetHandlerMap[keyword](cmdW);
			}
		}
	}


	recvBuffer.rewind();
}

