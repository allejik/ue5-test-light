// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/PlayerHUD.h"
#include "Components/ProgressBar.h"

void UPlayerHUD::SetHealthBar(const float CurrentHealth, const float MaxHealth)
{
	if (HealthBar) {
		HealthBar->SetPercent(CurrentHealth / MaxHealth);
	}
}
