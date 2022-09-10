// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/BaseHUD.h"

#include "Game/PlayerMenuUserWidget.h"
#include "Game/PlayerUserWidget.h"
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

void ABaseHUD::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	APlayerController* OwningPlayerController = GetOwningPlayerController();

	if (OwningPlayerController->WasInputKeyJustPressed(EKeys::T))
	{
		AddMenuUserWidget();
	}

	if (IsPlayerMenuUserOpen && !PlayerMenuUserWidget->IsVisible()) {
		PlayerMenuUserWidget->RemoveFromParent();
		PlayerMenuUserWidget = nullptr;
		
		// Enables/Disables cursor and click events
		OwningPlayerController->bShowMouseCursor = false; 
		OwningPlayerController->bEnableClickEvents = false; 
		OwningPlayerController->bEnableMouseOverEvents = false;
	
		// Enables/Disables player movement
		OwningPlayerController->SetIgnoreLookInput(false);
		OwningPlayerController->SetIgnoreMoveInput(false);

		IsPlayerMenuUserOpen = false;
	}
}

void ABaseHUD::AddMenuUserWidget()
{
	if (PlayerMenuUserWidgetClass) {
		APlayerController* OwningPlayerController = GetOwningPlayerController();

		// Enables/Disables cursor and click events
		OwningPlayerController->bShowMouseCursor = true; 
		OwningPlayerController->bEnableClickEvents = true; 
		OwningPlayerController->bEnableMouseOverEvents = true;
	
		// Enables/Disables player movement
		OwningPlayerController->SetIgnoreLookInput(true);
		OwningPlayerController->SetIgnoreMoveInput(true);

		// Sets cursor location in the center of the screen
		FVector2D ScreenSize;
		GEngine->GameViewport->GetViewportSize(ScreenSize);
		OwningPlayerController->SetMouseLocation(ScreenSize.X / 2, ScreenSize.Y / 2);

		PlayerMenuUserWidget = CreateWidget<UPlayerMenuUserWidget>(GetOwningPlayerController(), PlayerMenuUserWidgetClass);
		check(PlayerMenuUserWidget != nullptr);

		IsPlayerMenuUserOpen = true;
		PlayerMenuUserWidget->AddToPlayerScreen();
	}
}
