// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ChattingClient.h"
#include "Blueprint/UserWidget.h"
#include "ChattingLobby.generated.h"


UCLASS(blueprintable)
class CHATTINGCLIENT_API UChattingLobby : public UUserWidget
{
	GENERATED_BODY()

private:
	virtual void NativeConstruct() override final;

public:	
	UFUNCTION(BlueprintImplementableEvent, Category = "UMG")
	void AddChatMsgInLobby(const FString& fst, bool isMine);

	UFUNCTION(BlueprintImplementableEvent, Category = "UMG")
	void AddRoomInfoInLobby(const FString& fst);

	UFUNCTION(BlueprintImplementableEvent, Category = "UMG")
	void AddUserInfoInLobby(const FString& fst);

	UFUNCTION(BlueprintImplementableEvent, Category = "UMG")
	void ClearUserInfoList();

	UFUNCTION(BlueprintImplementableEvent, Category = "UMG")
	void ClearRoomInfoList();


/// <summary>
///  ���� ���� �ڵ�.
/// </summary>
public:
	virtual void RemoveFromParent() override;
protected:
	//@note: ������ �ȿ����� ����ϴ� �Լ��Դϴ�.
	void SetEnableAutoDestroy(bool bInEnableAutoDestroy);
public:
	bool IsEnabledAutoDestroy()const;
private:

	bool bEnableAutoDestroy;
};
