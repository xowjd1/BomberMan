// Fill out your copyright notice in the Description page of Project Settings.


#include "WaitingUI.h"
#include "Components/Button.h"
#include "NetGameInstance.h"
void UWaitingUI::NativeConstruct()
{
	gi = GetGameInstance<UNetGameInstance>();

	btn_start->OnClicked.AddDynamic(this, &UWaitingUI::OnClickStartButton);
}

void UWaitingUI::OnClickStartButton()
{
	gi->OnCreatedBattleSession();
}
