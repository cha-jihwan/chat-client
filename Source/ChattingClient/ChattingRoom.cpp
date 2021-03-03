#include "ChattingRoom.h"
#include "ChattingClientInstance.h"

// Sets default values
void UChattingRoom::NativeConstruct()
{
	Super::NativeConstruct();

	UChattingClientInstance::room = this;
	ABLOG(Warning, "void UChattingRom::NativeConstruct() %p", (void*)this);
}


