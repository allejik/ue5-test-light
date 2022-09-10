// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "PlayerMenuUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class TESTLIGHT_API UPlayerMenuUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;

	UFUNCTION()
	virtual void OnClickedResumeGameButton();

	UFUNCTION()
	virtual void OnClickedStartLevelMenuButton();
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* ResumeGame;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* StartLevelMenu;

	UPROPERTY(EditAnywhere)
	FString StartLevelName = "";
};
