// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/CharacterAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"

UCharacterAnimInstance::UCharacterAnimInstance()
{
	Speed = 0;
	IsJumping = false;
}

void UCharacterAnimInstance::NativeUpdateAnimation(const float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	const APawn* PawnOwner = TryGetPawnOwner();
	check(PawnOwner != nullptr);

	Speed = PawnOwner->GetVelocity().Size();
	IsJumping = PawnOwner->GetMovementComponent()->IsFalling();
}
