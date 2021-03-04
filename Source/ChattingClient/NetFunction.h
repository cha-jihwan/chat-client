//// Fill out your copyright notice in the Description page of Project Settings.
//
//#pragma once
//
//#include "ChattingClient.h"
//#include "Kismet/BlueprintFunctionLibrary.h"
//#include "NetFunction.generated.h"
//
///**
// * 
// */
//UCLASS(Blueprintable)
//class CHATTINGCLIENT_API UNetFunction : public UBlueprintFunctionLibrary
//{
//	GENERATED_BODY()
//
//public:
//	UFUNCTION(BlueprintCallable)
//	static void TestCPPCallToBP(AActor* c);
//};

#include <string>
#include <locale>

std::wstring MbsToWcs(const std::string& str);
std::string WcsToMbs(const std::wstring& str);