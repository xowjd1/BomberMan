// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BombermanProtoGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class BOMBERMANPROTO_API ABombermanProtoGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	FString GetLatestDeathPlayerNickname();

private:
	int32 callNumber = 0;
};
