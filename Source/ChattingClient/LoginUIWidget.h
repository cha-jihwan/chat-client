// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ChattingClient.h"
#include "Blueprint/UserWidget.h"

#include "LoginUIWidget.generated.h"

/**
 * 
 */
UCLASS()
class CHATTINGCLIENT_API ULoginUIWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	virtual void NativeConstruct() override final;

public:
	//UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Meta = (BindWidget))
	//class UTextBlock* LoginText;

	//UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Meta = (BindWidget))
	//class UButton* LoginBtn;

	UFUNCTION(BlueprintCallable, Category = "UMG")//  for test
	static void DoLogin(UPARAM(ref) FText& text);
	UFUNCTION(BlueprintCallable, Category = "UMG")//  for test
	static void DoChat(UPARAM(ref) FText& text);
	UFUNCTION(BlueprintCallable, Category = "UMG")//  for test
	static void CreateRoom(UPARAM(ref) FText& text);
	UFUNCTION(BlueprintCallable, Category = "UMG")//  for test
	static void EnterRoom(UPARAM(ref) FText& text);
	UFUNCTION(BlueprintCallable, Category = "UMG")//  for test
	static void LeaveRoom();
	UFUNCTION(BlueprintCallable, Category = "UMG")//  
	static void SelectRoomList();
	UFUNCTION(BlueprintCallable, Category = "UMG")//  for test
	static void SelectUserListInRoom();
	UFUNCTION(BlueprintCallable, Category = "UMG")//  for test
	static void SelectUserList();
	UFUNCTION(BlueprintCallable, Category = "UMG")//  for test
	static void WhisperToUser(UPARAM(ref) FText& name, UPARAM(ref) FText& msg);

	UFUNCTION(BlueprintCallable, Category = "UMG")//  for test
	static void DoDisconnect();

	
	UFUNCTION(BlueprintCallable, Category = "UMG")
	void BindIpAndPort(const FString& fst);


	UFUNCTION(BlueprintCallable, Category = "UMG")// 
	static FString GetIDTextToFString(UPARAM(ref) FText& text);

	UFUNCTION(BlueprintImplementableEvent, Category = "UMG")
	void AddChatMsg(const FString& fst);

	UFUNCTION(BlueprintCallable, Category = "UMG")//  for test
	static void AddChatInLobby(UPARAM(ref) FText& text);



};
