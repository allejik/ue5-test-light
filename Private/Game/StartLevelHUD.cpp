// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/StartLevelHUD.h"
#include "Game/StartLevelMenuUserWidget.h"
#include "Blueprint/UserWidget.h"

void AStartLevelHUD::BeginPlay()
{
	APlayerController* OwningPlayer = GetOwningPlayerController();

	// Enables/Disables cursor and click events
	OwningPlayer->bShowMouseCursor = true; 
	OwningPlayer->bEnableClickEvents = true; 
	OwningPlayer->bEnableMouseOverEvents = true;

	// Sets cursor location in the center of the screen
	FVector2D ScreenSize;
	GEngine->GameViewport->GetViewportSize(ScreenSize);
	OwningPlayer->SetMouseLocation(ScreenSize.X / 2, ScreenSize.Y / 2);

	if (StartLevelMenuUserWidgetClass) {
		StartLevelMenuUserWidget = CreateWidget<UStartLevelMenuUserWidget>(GetOwningPlayerController(), StartLevelMenuUserWidgetClass);
		check(StartLevelMenuUserWidget != nullptr);

		StartLevelMenuUserWidget->AddToPlayerScreen();
	}
}