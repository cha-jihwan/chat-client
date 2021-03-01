// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ChattingClientHUD.generated.h"

UCLASS()
class AChattingClientHUD : public AHUD
{
	GENERATED_BODY()

public:
	AChattingClientHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

