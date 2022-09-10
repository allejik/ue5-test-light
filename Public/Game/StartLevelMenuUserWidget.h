// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "StartLevelMenuUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class TESTLIGHT_API UStartLevelMenuUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;

	UFUNCTION()
	virtual void SetMouseCursor(bool IsShowing);

	UFUNCTION()
	virtual void OnClickedStartGameButton();

	UFUNCTION()
	virtual void OnClickedExitGameButton();
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* StartGame;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* ExitGame;

	UPROPERTY(EditAnywhere)
	FString StartLevelName = "";

	bool IsStartLevel = true;
};
