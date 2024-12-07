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

		//����, �÷��̾� ��ŸƮ ������ �±װ� "First"��� Ȧ�� ��° �÷��̾� ��ŸƮ�� ��ȯ�Ѵ�.
		//�׷��� �ʴٸ�, ¦�� ��° �÷��̾� ��ŸƮ�� ��ȯ�Ѵ�.
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
//	// �÷��̾���� �����ϴ� �迭 ����
//	TArray<ABBMCharacter*> Players;
//
//	// �÷��̾� ������ �� �迭�� �߰�
//	for (TActorIterator<ABBMCharacter> It(GetWorld()); It; ++It)
//	{
//		ABBMCharacter* Player = *It;
//		Players.Add(Player);
//
//		UE_LOG(LogTemp, Warning, TEXT("on"));
//	}
//
//	// �÷��̾ �� �� ������ ��
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
