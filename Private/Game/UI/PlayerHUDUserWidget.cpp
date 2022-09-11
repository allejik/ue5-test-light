// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UI/PlayerHUDUserWidget.h"
#include "Components/ProgressBar.h"

void UPlayerHUDUserWidget::SetHealthBar(const float CurrentHealth, const float MaxHealth)
{
	if (HealthBar) {
		HealthBar->SetPercent(CurrentHealth / MaxHealth);
	}
}
