// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ChatItemData.generated.h"

/**
 * 
 */
UCLASS(blueprintable)
class CHATTINGCLIENT_API UChatItemData : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent, Category = "UMG")// 
	void BP_Tets(float k);

	UFUNCTION(BlueprintImplementableEvent, Category = "UMG")// 
		void do_tst(float qwe);

};
