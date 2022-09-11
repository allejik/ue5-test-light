// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/HUD/BaseHUD.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Game/UI/PlayerMenuUserWidget.h"
#include "Game/UI/PlayerHUDUserWidget.h"
#include "GameFramework/Character.h"

void ABaseHUD::BeginPlay()
{
	Super::BeginPlay();
	AddPlayerHUD();
}

void ABaseHUD::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	APlayerController* OwningPlayerController = GetOwningPlayerController();

	// Checks whether user pressed menu button
	if (!IsPlayerMenuUserOpen && (OwningPlayerController->WasInputKeyJustPressed(EKeys::Escape)
		|| OwningPlayerController->IsInputKeyDown(EKeys::LeftControl)
		&& OwningPlayerController->IsInputKeyDown(EKeys::Q))) {

		ToggleMenuUserWidget(OwningPlayerController);
	}

	// Checks whether user menu should be removed
	if (IsPlayerMenuUserOpen && !PlayerMenuUserWidget->IsVisible()) {
		ToggleMenuUserWidget(OwningPlayerController);
	}
}

void ABaseHUD::AddPlayerHUD()
{
	if (!PlayerHUDUserWidgetClass) {
		UE_LOG(LogTemp, Error, TEXT("PlayerHUDUserWidgetClass is not assigned to BaseHUD"));
		return;
	}
	
	APlayerController* OwningPlayerController = GetOwningPlayerController();

	PlayerHUDUserWidget = CreateWidget<UPlayerHUDUserWidget>(OwningPlayerController, PlayerHUDUserWidgetClass);
	if (!PlayerHUDUserWidget) {
		UE_LOG(LogTemp, Log, TEXT("Cannot create PlayerHUDUserWidget in BaseHUD"));
		return;
	}

	PlayerHUDUserWidget->AddToPlayerScreen();

	// Sets focus to Game 
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(OwningPlayerController);
}

void ABaseHUD::ToggleMenuUserWidget(APlayerController* OwningPlayerController)
{
	if (!PlayerMenuUserWidgetClass) {
		UE_LOG(LogTemp, Error, TEXT("PlayerMenuUserWidgetClass is not assigned to BaseHUD"));
		return;
	}

	// Enables/Disables cursor and click events
	OwningPlayerController->bShowMouseCursor = !IsPlayerMenuUserOpen; 
	OwningPlayerController->bEnableClickEvents = !IsPlayerMenuUserOpen; 
	OwningPlayerController->bEnableMouseOverEvents = !IsPlayerMenuUserOpen;
	
	// Enables/Disables player movement
	OwningPlayerController->SetIgnoreLookInput(!IsPlayerMenuUserOpen);
	OwningPlayerController->SetIgnoreMoveInput(!IsPlayerMenuUserOpen);
	
	if (!IsPlayerMenuUserOpen) {
		// Sets cursor location in the center of the screen
		FVector2D ScreenSize;
		GEngine->GameViewport->GetViewportSize(ScreenSize);
		OwningPlayerController->SetMouseLocation(ScreenSize.X / 2, ScreenSize.Y / 2);

		PlayerMenuUserWidget = CreateWidget<UPlayerMenuUserWidget>(GetOwningPlayerController(), PlayerMenuUserWidgetClass);
		if (PlayerMenuUserWidget == nullptr) {
			UE_LOG(LogTemp, Log, TEXT("Cannot create PlayerMenuUserWidget in BaseHUD"));
			return;
		}

		PlayerMenuUserWidget->AddToPlayerScreen();

		// Sets focus to UI 
		UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(OwningPlayerController, PlayerMenuUserWidget, EMouseLockMode::LockAlways);
	} else {
		PlayerMenuUserWidget->RemoveFromParent();
		PlayerMenuUserWidget = nullptr;

		// Sets focus to Game 
		UWidgetBlueprintLibrary::SetInputMode_GameOnly(OwningPlayerController);
	}

	IsPlayerMenuUserOpen = !IsPlayerMenuUserOpen;
}
