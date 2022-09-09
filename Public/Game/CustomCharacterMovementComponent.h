// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CustomCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class TESTLIGHT_API UCustomCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

protected:
	FVector2D PreviousWalkingDirection;
public:
	explicit UCustomCharacterMovementComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void CalcVelocity(float DeltaTime, float Friction, bool bFluid, float BrakingDeceleration) override;

	FVector2D WalkingDirection;
};
