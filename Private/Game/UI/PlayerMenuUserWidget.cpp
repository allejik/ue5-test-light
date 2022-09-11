// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UI/PlayerMenuUserWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

bool UPlayerMenuUserWidget::Initialize()
{
	const bool bSuccess = Super::Initialize();
	if (!bSuccess) return false;

	ResumeGame->OnClicked.AddDynamic(this, &UPlayerMenuUserWidget::OnClickedResumeGameButton);
	StartLevelMenu->OnClicked.AddDynamic(this, &UPlayerMenuUserWidget::OnClickedStartLevelMenuButton);

	return true;
}

void UPlayerMenuUserWidget::OnClickedResumeGameButton()
{
	RemoveFromViewport();
}

void UPlayerMenuUserWidget::OnClickedStartLevelMenuButton()
{
	if (StartLevelName.IsEmpty()) {
		UE_LOG(LogTemp, Error, TEXT("StartLevelName is not set for PlayerMenuUserWidget"));
		return;
	}

	UGameplayStatics::OpenLevel(GetWorld(), FName(*StartLevelName));
}
