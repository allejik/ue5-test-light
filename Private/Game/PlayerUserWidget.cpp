// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/PlayerUserWidget.h"
#include "Components/ProgressBar.h"

void UPlayerUserWidget::SetHealthBar(const float CurrentHealth, const float MaxHealth)
{
	if (HealthBar) {
		HealthBar->SetPercent(CurrentHealth / MaxHealth);
	}
}
