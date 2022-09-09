// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/MenuGameModeBase.h"
#include "Game/PlayerStartingMenu.h"

void AMenuGameModeBase::StartPlay()
{
	Super::StartPlay();
	AddPlayerMenu();
}

void AMenuGameModeBase::AddPlayerMenu()
{
	if (PlayerStartingMenuClass) {
		PlayerStartingMenu = CreateWidget<UPlayerStartingMenu>(GetWorld(), PlayerStartingMenuClass);
		check(PlayerStartingMenu != nullptr);

		PlayerStartingMenu->AddToPlayerScreen();
	}
}
