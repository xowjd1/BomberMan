// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginWidget.h"
#include "Components/Button.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Components/EditableText.h"
#include "NetGameInstance.h"
#include "Components/WidgetSwitcher.h"
#include "SessionInfoWidget.h"
#include "Components/ScrollBox.h"
#include "Components/ComboBoxString.h"

void ULoginWidget::NativeConstruct()
{
	gi = GetGameInstance<UNetGameInstance>();

	btn_CreateSession->OnClicked.AddDynamic(this, &ULoginWidget::OnClickCreateButton);
	slider_playerCount->OnValueChanged.AddDynamic(this, &ULoginWidget::OnSliderMoved);
	btn_FindSession->OnClicked.AddDynamic(this, &ULoginWidget::OnClickFindButton);
	btn_BackFromCreateSession->OnClicked.AddDynamic(this, &ULoginWidget::BackToFirstCanvas);
	btn_CreateSelection->OnClicked.AddDynamic(this, &ULoginWidget::OnClickedCreateSelection);
	cbs_selectColor->OnSelectionChanged.AddDynamic(this, &ULoginWidget::OnColorItemSelected);
	//btn_Next->OnClicked.AddDynamic(this, &ULoginWidget::OnClickNextButton);

	if (gi != nullptr)
	{
		gi->onSearchCompleted.AddDynamic(this, &ULoginWidget::AddRoomSlot);
		gi->onFindButtonActivation.AddDynamic(this, &ULoginWidget::ChangeButtonActivation);
	}
}

void ULoginWidget::OnClickCreateButton()
{
	if (gi != nullptr && !edit_roomName->GetText().IsEmpty())
	{
		gi->CreateMySession(edit_roomName->GetText(), (int32)slider_playerCount->GetValue());
	}
}

void ULoginWidget::OnClickedCreateSelection()
{
	if (!edit_userName->GetText().IsEmpty())
	{
		gi->mySessionName = edit_userName->GetText().ToString();
		gi->playerInitInfo.Set(edit_userName->GetText().ToString(), currentColorItem);
	}
	SwitchCanvas(1);
}

void ULoginWidget::OnClickFindButton()
{
	if (gi != nullptr)
	{
		gi->FindOtherSession();
		sb_RoomListBox->ClearChildren(); // 원래 있던 방 싹 날리고 find
	}
	if (!edit_userName->GetText().IsEmpty())
	{
		gi->mySessionName = edit_userName->GetText().ToString();
		gi->playerInitInfo.Set(edit_userName->GetText().ToString(), currentColorItem);
	}
}

void ULoginWidget::OnSliderMoved(float value)
{
	text_sliderCount->SetText(FText::AsNumber((int32)value));
}

void ULoginWidget::AddRoomSlot(struct FSessionSlotInfo slotInfo)
{
	// 슬롯 위젯을 생성한다.
	sessionSlot = CreateWidget<USessionInfoWidget>(GetWorld(), sessionInfoWidget);

	if (sessionSlot != nullptr)
	{
		// 슬롯 위젯의 각 요소에 슬롯 정보를 넣는다.
		sessionSlot->text_RoomName->SetText(FText::FromString(slotInfo.roomName));
		sessionSlot->text_PlayerCount->SetText(FText::FromString(slotInfo.playerCount));
		sessionSlot->sessionIndex = slotInfo.sessionIndex;

		// 생성한 슬롯 위젯을 스크롤 박스에 자식으로 추가한다.
		sb_RoomListBox->AddChild(sessionSlot);
	}
}

void ULoginWidget::BackToFirstCanvas()
{
	ws_SessionUISwitch->SetActiveWidgetIndex(0);
}

void ULoginWidget::ChangeButtonActivation(bool bIsActivation)
{
	btn_FindSession->SetIsEnabled(bIsActivation);
}

void ULoginWidget::SwitchCanvas(int32 index)
{
	ws_SessionUISwitch->SetActiveWidgetIndex(index);
}

void ULoginWidget::OnColorItemSelected(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (SelectionType == ESelectInfo::OnMouseClick || SelectionType == ESelectInfo::OnKeyPress)
	{
		if (SelectedItem == "White")
		{
			currentColorItem = FLinearColor::White;
		}
		else if (SelectedItem == "Red")
		{
			currentColorItem = FLinearColor::Red;
		}
		else if (SelectedItem == "Green")
		{
			currentColorItem = FLinearColor::Green;
		}
		else if (SelectedItem == "Black")
		{
			currentColorItem = FLinearColor::Black;
		}
		else if (SelectedItem == "Blue")
		{
			currentColorItem = FLinearColor::Blue;
		}
		else if (SelectedItem == "Yellow")
		{
			currentColorItem = FLinearColor::Yellow;
		}
	}
}

//void ULoginWidget::OnClickNextButton()
//{
//
//}
