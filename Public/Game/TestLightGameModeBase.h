// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TestLightGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class TESTLIGHT_API ATestLightGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void StartPlay() override;
};
