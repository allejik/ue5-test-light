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

	// Player's HUD
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UPlayerUserWidget> PlayerUserWidgetClass;

	UPROPERTY()
	UPlayerUserWidget* PlayerUserWidget;
};
