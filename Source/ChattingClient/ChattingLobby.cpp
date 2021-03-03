#include "ChattingLobby.h"
#include "Blueprint/WidgetTree.h"
#include "ChattingClientInstance.h"


// Sets default values
void UChattingLobby::NativeConstruct()
{
	Super::NativeConstruct();

	//ConstructorDetector Detector(this);

	SetEnableAutoDestroy(false);

	UChattingClientInstance::lobby = this;
	ABLOG(Warning, "void UChattingLobby::NativeConstruct() %p", (void*)this);
}

void UChattingLobby::RemoveFromParent()
{
	if (bEnableAutoDestroy)
	{
		Super::RemoveFromParent();
	}
}

void UChattingLobby::SetEnableAutoDestroy(bool bInEnableAutoDestroy)
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

bool UChattingLobby::IsEnabledAutoDestroy() const
{
	return bEnableAutoDestroy;
}
