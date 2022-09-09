// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MenuGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class TESTLIGHT_API AMenuGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void AddPlayerMenu();

public:
	virtual void StartPlay() override;

	// Player Menu
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UPlayerStartingMenu> PlayerStartingMenuClass;

	UPROPERTY()
	UPlayerStartingMenu* PlayerStartingMenu;
};
