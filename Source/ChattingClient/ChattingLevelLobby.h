#pragma once

#include "ChattingClient.h"
#include "ChattingLevel.h"
#include "ChattingLevelLobby.generated.h"

/**
 * 
 */
UCLASS()
class CHATTINGCLIENT_API UChattingLevelLobby : public UChattingLevel
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override final;
};
