// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/BaseHUD.h"
#include "Game/PlayerUserWidget.h"
#include "Game/StartLevelMenuUserWidget.h"
#include "GameFramework/Character.h"

void ABaseHUD::BeginPlay()
{
	APlayerController* OwningPlayerController = GetOwningPlayerController();

	if (PlayerUserWidgetClass) {
		PlayerUserWidget = CreateWidget<UPlayerUserWidget>(OwningPlayerController, PlayerUserWidgetClass);
		check(PlayerUserWidget != nullptr);

		PlayerUserWidget->AddToPlayerScreen();
	}
}

void ABaseHUD::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (GetOwningPlayerController()->WasInputKeyJustPressed(EKeys::T))
	{
		AddMenuUserWidget();
	}
}

void ABaseHUD::AddMenuUserWidget()
{
	if (StartLevelMenuUserWidget) {
		StartLevelMenuUserWidget->RemoveFromParent();
		StartLevelMenuUserWidget = nullptr;
	}

	if (StartLevelMenuUserWidgetClass) {
		StartLevelMenuUserWidget = CreateWidget<UStartLevelMenuUserWidget>(GetOwningPlayerController(), StartLevelMenuUserWidgetClass);
		check(StartLevelMenuUserWidget != nullptr);
		StartLevelMenuUserWidget->IsStartLevel = false;
		StartLevelMenuUserWidget->AddToPlayerScreen();
	}
}
