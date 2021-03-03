// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChattingRoom.generated.h"

/**
 * 
 */
UCLASS()
class CHATTINGCLIENT_API UChattingRoom : public UUserWidget
{
	GENERATED_BODY()

private:
	virtual void NativeConstruct() override final;

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "UMG")
	void AddChatMsgInRoom(const FString& fst, bool isMine);

	UFUNCTION(BlueprintImplementableEvent, Category = "UMG")
	void AddUserInfoInRoom(const FString& fst);

	UFUNCTION(BlueprintImplementableEvent, Category = "UMG")
	void ClearUserInfoList();
};
