// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BaseHUD.generated.h"

/**
 * 
 */
UCLASS()
class TESTLIGHT_API ABaseHUD : public AHUD
{
	GENERATED_BODY()

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION()
	virtual void AddMenuUserWidget();

	// HUD Widget
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UPlayerHUDUserWidget> PlayerUserWidgetClass;

	UPROPERTY()
	UPlayerHUDUserWidget* PlayerUserWidget;

	// Player Menu Widget
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UPlayerMenuUserWidget> PlayerMenuUserWidgetClass;

	UPROPERTY()
	UPlayerMenuUserWidget* PlayerMenuUserWidget;

	UPROPERTY()
	bool IsPlayerMenuUserOpen = false;
};
