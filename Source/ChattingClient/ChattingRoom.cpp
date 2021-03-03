#include "ChattingRoom.h"
#include "ChattingClientInstance.h"

// Sets default values
void UChattingRoom::NativeConstruct()
{
	Super::NativeConstruct();

	SetEnableAutoDestroy(false);

	UChattingClientInstance::room = this;
	ABLOG(Warning, "void UChattingRom::NativeConstruct() %p", (void*)this);
}


void UChattingRoom::RemoveFromParent()
{
	if (bEnableAutoDestroy)
	{
		Super::RemoveFromParent();
	}
}

void UChattingRoom::SetEnableAutoDestroy(bool bInEnableAutoDestroy)
{
	bEnableAutoDestroy = bInEnableAutoDestroy;

	int32 CurrentFlags = (int32)(GetFlags());
	if (bEnableAutoDestroy)
	{
		SetFlags((EObjectFlags)(CurrentFlags | RF_MarkAsRootSet));
	}
	else
	{
		SetFlags((EObjectFlags)(CurrentFlags ^ RF_MarkAsRootSet));
	}
}

bool UChattingRoom::IsEnabledAutoDestroy() const
{
	return bEnableAutoDestroy;
}
