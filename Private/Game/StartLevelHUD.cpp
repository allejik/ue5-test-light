// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/StartLevelHUD.h"
#include "Game/StartLevelMenuUserWidget.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void AStartLevelHUD::BeginPlay()
{
	APlayerController* OwningPlayerController = GetOwningPlayerController();

	// Enables/Disables cursor and click events
	OwningPlayerController->bShowMouseCursor = true; 
	OwningPlayerController->bEnableClickEvents = true; 
	OwningPlayerController->bEnableMouseOverEvents = true;

	// Sets cursor location in the center of the screen
	FVector2D ScreenSize;
	GEngine->GameViewport->GetViewportSize(ScreenSize);
	OwningPlayerController->SetMouseLocation(ScreenSize.X / 2, ScreenSize.Y / 2);

	if (StartLevelMenuUserWidgetClass) {
		StartLevelMenuUserWidget = CreateWidget<UStartLevelMenuUserWidget>(GetOwningPlayerController(), StartLevelMenuUserWidgetClass);
		check(StartLevelMenuUserWidget != nullptr);

		StartLevelMenuUserWidget->AddToPlayerScreen();

		// Sets focus to UI 
		UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(OwningPlayerController, StartLevelMenuUserWidget, EMouseLockMode::LockAlways);
	}
}
