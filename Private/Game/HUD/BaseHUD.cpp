// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/HUD/BaseHUD.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Game/UI/PlayerMenuUserWidget.h"
#include "Game/UI/PlayerHUDUserWidget.h"
#include "GameFramework/Character.h"

void ABaseHUD::BeginPlay()
{
	APlayerController* OwningPlayerController = GetOwningPlayerController();

	if (PlayerUserWidgetClass) {
		PlayerUserWidget = CreateWidget<UPlayerHUDUserWidget>(OwningPlayerController, PlayerUserWidgetClass);
		check(PlayerUserWidget != nullptr);

		PlayerUserWidget->AddToPlayerScreen();

		// Sets focus to Game 
		UWidgetBlueprintLibrary::SetInputMode_GameOnly(OwningPlayerController);
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

		// Sets focus to Game 
		UWidgetBlueprintLibrary::SetInputMode_GameOnly(OwningPlayerController);
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

		// Sets focus to UI 
		UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(OwningPlayerController, PlayerMenuUserWidget, EMouseLockMode::LockAlways);
	}
}
