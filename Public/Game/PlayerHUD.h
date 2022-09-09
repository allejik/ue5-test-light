// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUD.generated.h"

/**
 * 
 */
UCLASS()
class TESTLIGHT_API UPlayerHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetHealthBar(float CurrentHealth, float MaxHealth);

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UProgressBar* HealthBar;
};
