// Fill out your copyright notice in the Description page of Project Settings.


#include "ChattingLobby.h"
#include "Blueprint/WidgetTree.h"
#include "ChattingClientInstance.h"


// Sets default values
void UChattingLobby::NativeConstruct()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	Super::NativeConstruct();


	UChattingClientInstance::lobby = this;
	UChattingClientInstance::initialized = false;
}


