// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/HUD/StartLevelHUD.h"
#include "Game/UI/StartLevelMenuUserWidget.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void AStartLevelHUD::BeginPlay()
{
	APlayerController* OwningPlayerController = GetOwningPlayerController();

	// Enables/Disables cursor and click events
	OwningPlayerController->bShowMouseCursor = true; 
	OwningPlayerController->bEnableClickEvents = true; 
	OwningPlayerController->bEnableMouseOverEvents = true;

	if (StartLevelMenuUserWidgetClass) {
		StartLevelMenuUserWidget = CreateWidget<UStartLevelMenuUserWidget>(GetOwningPlayerController(), StartLevelMenuUserWidgetClass);
		check(StartLevelMenuUserWidget != nullptr);

		StartLevelMenuUserWidget->AddToPlayerScreen();

		// Sets focus to UI 
		UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(OwningPlayerController, StartLevelMenuUserWidget, EMouseLockMode::LockAlways);
	}
}
