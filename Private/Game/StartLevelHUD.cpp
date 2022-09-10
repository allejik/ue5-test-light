// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/StartLevelHUD.h"
#include "Game/StartLevelMenuUserWidget.h"
#include "Blueprint/UserWidget.h"

void AStartLevelHUD::BeginPlay()
{
	if (StartLevelMenuUserWidgetClass) {
		StartLevelMenuUserWidget = CreateWidget<UStartLevelMenuUserWidget>(GetOwningPlayerController(), StartLevelMenuUserWidgetClass);
		check(StartLevelMenuUserWidget != nullptr);

		StartLevelMenuUserWidget->AddToPlayerScreen();
	}
}