// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/BaseHUD.h"
#include "Game/PlayerUserWidget.h"

void ABaseHUD::BeginPlay()
{
	if (PlayerUserWidgetClass) {
		PlayerUserWidget = CreateWidget<UPlayerUserWidget>(GetOwningPlayerController(), PlayerUserWidgetClass);
		check(PlayerUserWidget != nullptr);

		PlayerUserWidget->AddToPlayerScreen();
	}
}
