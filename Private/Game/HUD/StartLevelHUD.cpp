// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/HUD/StartLevelHUD.h"
#include "Game/UI/StartLevelMenuUserWidget.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"\

void AStartLevelHUD::BeginPlay()
{
	Super::BeginPlay();
	AddMenu();
}

void AStartLevelHUD::AddMenu()
{
	if (!StartLevelMenuUserWidgetClass) {
		UE_LOG(LogTemp, Error, TEXT("StartLevelMenuUserWidgetClass is not assigned to StartLevelHUD"));
		return;
	}

	APlayerController* OwningPlayerController = GetOwningPlayerController();

	// Enables/Disables cursor and click events
	OwningPlayerController->bShowMouseCursor = true; 
	OwningPlayerController->bEnableClickEvents = true; 
	OwningPlayerController->bEnableMouseOverEvents = true;

	StartLevelMenuUserWidget = CreateWidget<UStartLevelMenuUserWidget>(GetOwningPlayerController(), StartLevelMenuUserWidgetClass);
	if (!StartLevelMenuUserWidget) {
		UE_LOG(LogTemp, Log, TEXT("Cannot create StartLevelMenuUserWidget in StartLevelHUD"));
		return;
	}

	StartLevelMenuUserWidget->AddToPlayerScreen();

	// Sets focus to UI 
	UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(OwningPlayerController, StartLevelMenuUserWidget, EMouseLockMode::LockAlways);
}
