// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ChattingClient.h"
#include "ChattingLevel.h"
#include "ChattingLevelRoom.generated.h"

/**
 * 
 */
UCLASS()
class CHATTINGCLIENT_API UChattingLevelRoom : public UChattingLevel
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override final;
};
