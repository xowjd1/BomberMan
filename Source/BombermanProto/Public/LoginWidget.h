// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoginWidget.generated.h"

/**
 * 
 */
UCLASS()
class BOMBERMANPROTO_API ULoginWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
private:
	UPROPERTY()
	class USessionInfoWidget* sessionSlot;

	class UNetGameInstance* gi;

public:
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = MySettings)
	class UEditableText* edit_roomName;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = MySettings)
	class USlider* slider_playerCount;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = MySettings)
	class UTextBlock* text_sliderCount;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = MySettings)
	class UButton* btn_CreateSession;

	UPROPERTY(VisibleAnywhere, meta=(Bindwidget), Category=MySettings)
	class UButton* btn_FindSession;

	UPROPERTY(VisibleAnywhere, meta = (Bindwidget), Category = MySettings)
	class UButton* btn_CreateSelection;

	UPROPERTY(VisibleAnywhere, meta = (Bindwidget), Category = MySettings)
	class UButton* btn_BackFromCreateSession;

	UPROPERTY(VisibleAnywhere, meta=(Bindwidget), Category=MySettings)
	class UScrollBox* sb_RoomListBox;

	UPROPERTY(VisibleAnywhere, meta=(Bindwidget), Category=MySettings)
	class UWidgetSwitcher* ws_SessionUISwitch;

	UPROPERTY(VisibleAnywhere, meta = (Bindwidget), Category = MySettings)
	class UComboBoxString* cbs_selectColor;
	/*UPROPERTY(VisibleAnywhere, meta = (Bindwidget), Category = MySettings)
	class UButton* btn_Next;*/

	UPROPERTY(EditAnywhere, Category=MySettings)
	TSubclassOf<class USessionInfoWidget> sessionInfoWidget;

	UPROPERTY(VisibleAnywhere, meta = (Bindwidget), Category = MySettings)
	class UEditableText* edit_userName;

	FLinearColor currentColorItem;

	UFUNCTION()
	void OnClickCreateButton();

	UFUNCTION()
	void OnClickedCreateSelection();
	
	UFUNCTION()
	void OnClickFindButton();
	
	UFUNCTION()
	void OnSliderMoved(float value);

	UFUNCTION()
	void AddRoomSlot(struct FSessionSlotInfo slotInfo);

	UFUNCTION()
	void BackToFirstCanvas();

	UFUNCTION()
	void ChangeButtonActivation(bool bIsActivation);

	void SwitchCanvas(int32 index);

	UFUNCTION()
	void OnColorItemSelected(FString SelectedItem, ESelectInfo::Type SelectionType);

	//UFUNCTION()
	//void OnClickNextButton();
};
