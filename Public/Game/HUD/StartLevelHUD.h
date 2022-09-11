// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "StartLevelHUD.generated.h"

/**
 * 
 */
UCLASS()
class TESTLIGHT_API AStartLevelHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UStartLevelMenuUserWidget> StartLevelMenuUserWidgetClass;

	UPROPERTY()
	UStartLevelMenuUserWidget* StartLevelMenuUserWidget;
};
