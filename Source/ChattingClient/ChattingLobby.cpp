#include "ChattingLobby.h"
#include "Blueprint/WidgetTree.h"
#include "ChattingClientInstance.h"


// Sets default values
void UChattingLobby::NativeConstruct()
{
	Super::NativeConstruct();

	UChattingClientInstance::lobby = this;
}

