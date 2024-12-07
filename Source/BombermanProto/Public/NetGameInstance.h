// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "NetGameInstance.generated.h"


/**
 * 
 */

USTRUCT(BlueprintType)
struct FSessionSlotInfo
{
	GENERATED_USTRUCT_BODY()

public:
	FString roomName = TEXT("");
	FString playerCount = TEXT("");
	int32 sessionIndex = 0;

	FORCEINLINE void Set(FString rName, FString pCount, int32 index) { roomName = rName; playerCount = pCount;  sessionIndex = index; }
};

USTRUCT(BlueprintType)
struct FPlayerInitInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	FString userName = TEXT("");
	UPROPERTY()
	FLinearColor meshColor = FLinearColor::White;

	FORCEINLINE void Set(FString name, FLinearColor color)
	{
		userName = name;
		meshColor = color;
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSearchSignature, FSessionSlotInfo, sessionInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFindButtonSignature, bool, bIsActivation);

UCLASS()
class BOMBERMANPROTO_API UNetGameInstance : public UGameInstance
{
	GENERATED_BODY()
	

public:
	UNetGameInstance();

	virtual void Init() override;

	UPROPERTY()
		FString mySessionName;

	IOnlineSessionPtr sessionInterface;
	TSharedPtr<FOnlineSessionSearch> sessionSearch;
	FSearchSignature onSearchCompleted;
	FFindButtonSignature onFindButtonActivation;
	FPlayerInitInfo playerInitInfo;

	bool isclicked = false;

	void CreateMySession(FText roomName, int32 playerCount);
	void OnCreatedMySession(FName sessionName, bool bWasSuccessful);
	void OnCreatedBattleSession();
	void FindOtherSession();
	void OnFindOtherSessions(bool bWasSuccessful);
	void JoinSelectedSession(int32 index);
	void OnJoinSelectedSession(FName sessionName, EOnJoinSessionCompleteResult::Type result);
};
