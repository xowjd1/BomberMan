// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NetGameInstance.h"
#include "WaitingUI.generated.h"

/**
 * 
 */
UCLASS()
class BOMBERMANPROTO_API UWaitingUI : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

private:
	class UNetGameInstance* gi;

public:

	UPROPERTY(VisibleAnywhere, meta=(Bindwidget), Category=MySettings)
	class UButton* btn_start;

	UPROPERTY(VisibleAnywhere, meta=(Bindwidget), Category=MySettings)
	class UImage* img_start;

	UFUNCTION()
	void OnClickStartButton();

	
};
