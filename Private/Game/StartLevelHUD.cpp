// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/StartLevelHUD.h"
#include "Game/StartLevelMenuUserWidget.h"
#include "Blueprint/UserWidget.h"

void AStartLevelHUD::BeginPlay()
{
	APlayerController* OwningPlayerController = GetOwningPlayerController();
	OwningPlayerController->bShowMouseCursor = true; 
	OwningPlayerController->bEnableClickEvents = true; 
	OwningPlayerController->bEnableMouseOverEvents = true;

	if (StartLevelMenuUserWidgetClass) {
		StartLevelMenuUserWidget = CreateWidget<UStartLevelMenuUserWidget>(OwningPlayerController, StartLevelMenuUserWidgetClass);
		check(StartLevelMenuUserWidget != nullptr);

		StartLevelMenuUserWidget->AddToPlayerScreen();
	}
}