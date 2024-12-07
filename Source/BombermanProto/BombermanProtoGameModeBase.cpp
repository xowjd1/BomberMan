// Copyright Epic Games, Inc. All Rights Reserved.


#include "BombermanProtoGameModeBase.h"
#include <GameFramework/PlayerStart.h>
#include "EngineUtils.h"
#include <Kismet/GameplayStatics.h>
#include "BBMCharacter.h"
#include <UMG/Public/Blueprint/UserWidget.h>
#include "ScoreWidget.h"

AActor* ABombermanProtoGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
	TArray<APlayerStart*> playerStarts;
	playerStarts.SetNum(6);

	for (TActorIterator<APlayerStart> it(GetWorld()); it; ++it)
	{
		APlayerStart* ps = *it;

		//만일, 플레이어 스타트 액터의 태그가 "First"라면 홀수 번째 플레이어 스타트로 반환한다.
		//그렇지 않다면, 짝수 번째 플레이어 스타트로 반환한다.
		if (ps->ActorHasTag(FName("first")))
		{
			playerStarts[0] = ps;
		}
		else if (ps->ActorHasTag(FName("second")))
		{
			playerStarts[1] = ps;
		}
		else if (ps->ActorHasTag(FName("third")))
		{
			playerStarts[2] = ps;
		}
		else if (ps->ActorHasTag(FName("fourth")))
		{
			playerStarts[3] = ps;
		}
		else if (ps->ActorHasTag(FName("fifth")))
		{
			playerStarts[4] = ps;
		}
		else
		{
			playerStarts[5] = ps;
		}
	}
	APlayerStart* chooseStart = playerStarts[callNumber];
	//callNumber = (callNumber + 1) % playerStarts.Num();
	callNumber++;
	return chooseStart;
}
//
//FString ABombermanProtoGameModeBase::GetLatestDeathPlayerNickname()
//{
//	// 플레이어들을 추적하는 배열 생성
//	TArray<ABBMCharacter*> Players;
//
//	// 플레이어 생성할 때 배열에 추가
//	for (TActorIterator<ABBMCharacter> It(GetWorld()); It; ++It)
//	{
//		ABBMCharacter* Player = *It;
//		Players.Add(Player);
//
//		UE_LOG(LogTemp, Warning, TEXT("on"));
//	}
//
//	// 플레이어가 한 명만 남았을 때
//	if (Players.Num() == 1)
//	{
//		ABBMCharacter* LastPlayer = Players[0];
//		FString LastPlayerName = LastPlayer->myName;
//
//		UE_LOG(LogTemp, Warning, TEXT("Last player remaining: %s"), *LastPlayerName);
//
//		LastPlayer->isScoreCheck = true;
//	}
//
//	return FString();
//}
