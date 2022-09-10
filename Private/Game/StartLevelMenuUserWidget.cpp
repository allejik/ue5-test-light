// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/StartLevelMenuUserWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

bool UStartLevelMenuUserWidget::Initialize()
{
	const bool bSuccess = Super::Initialize();
	if (!bSuccess) return false;

	SetMouseCursor(true);

	StartGame->OnClicked.AddDynamic(this, &UStartLevelMenuUserWidget::UStartLevelMenuUserWidget::OnClickedStartGameButton);
	ExitGame->OnClicked.AddDynamic(this, &UStartLevelMenuUserWidget::OnClickedExitGameButton);

	return true;
}

void UStartLevelMenuUserWidget::OnClickedStartGameButton()
{
	SetMouseCursor(false);

	if (!IsStartLevel) {
		RemoveFromParent();
		return;
	}
	
	if (StartLevelName.IsEmpty()) {
		UE_LOG(LogTemp, Error, TEXT("StartLevelName is not set for StartLevelMenu"));
		return;
	}

	UGameplayStatics::OpenLevel(GetWorld(), FName(*StartLevelName));
}

void UStartLevelMenuUserWidget::SetMouseCursor(const bool IsShowing)
{
	APlayerController* OwningPlayer = GetOwningPlayer();
	OwningPlayer->bShowMouseCursor = IsShowing; 
	OwningPlayer->bEnableClickEvents = IsShowing; 
	OwningPlayer->bEnableMouseOverEvents = IsShowing;
	OwningPlayer->SetIgnoreLookInput(IsShowing);
	OwningPlayer->SetIgnoreMoveInput(IsShowing);
}

void UStartLevelMenuUserWidget::OnClickedExitGameButton()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}

