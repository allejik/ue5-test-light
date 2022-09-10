// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/StartLevelMenuUserWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

bool UStartLevelMenuUserWidget::Initialize()
{
	const bool bSuccess = Super::Initialize();
	if (!bSuccess) return false;

	StartGame->OnClicked.AddDynamic(this, &UStartLevelMenuUserWidget::UStartLevelMenuUserWidget::OnClickedStartGameButton);
	ExitGame->OnClicked.AddDynamic(this, &UStartLevelMenuUserWidget::OnClickedExitGameButton);

	return true;
}

void UStartLevelMenuUserWidget::OnClickedStartGameButton()
{
	if (StartLevelName.IsEmpty()) {
		UE_LOG(LogTemp, Error, TEXT("StartLevelName is not set for StartLevelMenuUserWidget"));
		return;
	}

	UGameplayStatics::OpenLevel(GetWorld(), FName(*StartLevelName));
}

void UStartLevelMenuUserWidget::OnClickedExitGameButton()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}

