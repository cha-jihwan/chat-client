// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginUIWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Blueprint/WidgetTree.h"
#include "ChattingClientInstance.h"
#include "NetworkManager.h"
#include <string>

void ULoginUIWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UChattingClientInstance::lobby = this;
	UChattingClientInstance::initialized = false;
	/*LoginBtn = WidgetTree->FindWidget<UButton>("LoginBtn");
	LoginText = WidgetTree->FindWidget<UTextBlock>("LoginText");

	if (LoginText != nullptr)
	{
		ABLOG(Warning, TEXT("LoginTxt != nullptr"));
	}
	else
	{
		ABLOG(Warning, TEXT("LoginTxt == nullptr"));
	}*/
	//UTextBlock* MyTitleTextBlock = this->WidgetTree->FindWidget<UTextBlock>("LoginTextBox");
	//if (MyTitleTextBlock)
	//{
	//	FString str(TEXT("차지환"));
	//	MyTitleTextBlock->SetText(FText::FromString(str));
	//}

}

// 블루프린트로 글자 제한 함.
void ULoginUIWidget::DoLogin(UPARAM(ref) FText& ftext)
{
	FString fstr = ftext.ToString();
	char buffer[1024]{};

	size_t retSize = std::wcstombs(buffer, *fstr, 1024);

	std::string loginCmd = "/l ";
	loginCmd += buffer;
	loginCmd += "\r\n";

	UChattingClientInstance::RequestSending(const_cast<char*>(loginCmd.c_str()), loginCmd.size());

	/*FString levelLobby = TEXT("LevelLobby");
	UChattingClientInstance::ChangeLevel(levelLobby);*/

}

// 블루프린트로 글자 제한 함.
void ULoginUIWidget::DoChat(UPARAM(ref) FText& ftext)
{
	FString fstr = ftext.ToString();
	char buffer[1024]{};
	
	size_t retSize = std::wcstombs(buffer, *fstr, 1024);

	std::string chatCmd = "/c ";
	chatCmd += buffer;
	chatCmd += "\r\n";

	UChattingClientInstance::GetNetManager()->PreSend(const_cast<char*>(chatCmd.c_str()), chatCmd.size());
}


void ULoginUIWidget::CreateRoom(UPARAM(ref) FText& ftext)
{
	FString fstr = ftext.ToString();
	
	char buffer[1024]{};

	size_t retSize = std::wcstombs(buffer, *fstr, 1024);

	std::string createRoomCmd = "/r ";
	createRoomCmd += buffer;
	createRoomCmd += "\r\n";

	UChattingClientInstance::GetNetManager()->PreSend(const_cast<char*>(createRoomCmd.c_str()), createRoomCmd.size());

	
}

void ULoginUIWidget::EnterRoom(UPARAM(ref) FText& ftext)
{
	FString fstr = ftext.ToString();
	
	char buffer[1024]{};

	
	size_t retSize = std::wcstombs(buffer, *fstr, 1024);

	std::string enterRoomCmd = "/q ";
	enterRoomCmd += buffer;
	enterRoomCmd += "\r\n";

	UChattingClientInstance::GetNetManager()->PreSend(const_cast<char*>(enterRoomCmd.c_str()), enterRoomCmd.size());
}

void ULoginUIWidget::LeaveRoom()
{
	std::string leaveRoomCmd = "/e\r\n";

	UChattingClientInstance::GetNetManager()->PreSend(const_cast<char*>(leaveRoomCmd.c_str()), leaveRoomCmd.size());
}

void ULoginUIWidget::SelectRoomList()
{
	std::string selectRoomListCmd = "/s\r\n";

	UChattingClientInstance::GetNetManager()->PreSend(const_cast<char*>(selectRoomListCmd.c_str()), selectRoomListCmd.size());
}

void ULoginUIWidget::SelectUserListInRoom()
{
	std::string selectUserInRoomCmd = "/u\r\n";

	UChattingClientInstance::GetNetManager()->PreSend(const_cast<char*>(selectUserInRoomCmd.c_str()), selectUserInRoomCmd.size());
}

void ULoginUIWidget::SelectUserList()
{
	std::string selectUserListCmd = "/a\r\n";

	UChattingClientInstance::GetNetManager()->PreSend(const_cast<char*>(selectUserListCmd.c_str()), selectUserListCmd.size());
}

void ULoginUIWidget::WhisperToUser(UPARAM(ref)FText& ftext_name, UPARAM(ref) FText& ftext_msg)
{
	char buffer[1024]{};
	FString fstr = ftext_name.ToString() + " " + ftext_msg.ToString();
	size_t retSize = std::wcstombs(buffer, *fstr, 1024);

	std::string whisperToUserCmd = "/w ";
	whisperToUserCmd += buffer;
	whisperToUserCmd += "\r\n";

	UChattingClientInstance::GetNetManager()->PreSend(const_cast<char*>(whisperToUserCmd.c_str()), whisperToUserCmd.size());
}


FString ULoginUIWidget::GetIDTextToFString(UPARAM(ref) FText& text)
{


	return text.ToString();
}

void ULoginUIWidget::AddChat(UPARAM(ref) FText& text)
{
	if (nullptr != UChattingClientInstance::GetLobby())
	{
		UChattingClientInstance::GetLobby()->AddChatMsg(text.ToString());
	}
}
