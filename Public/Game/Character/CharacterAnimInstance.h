// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class TESTLIGHT_API UCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	explicit UCharacterAnimInstance();

	UPROPERTY(BlueprintReadOnly)
	float Speed;

	UPROPERTY(BlueprintReadOnly)
	bool IsJumping;

	UPROPERTY(BlueprintReadOnly)
	bool IsCrouching;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
