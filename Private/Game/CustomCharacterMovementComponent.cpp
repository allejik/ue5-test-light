// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/CustomCharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"

UCustomCharacterMovementComponent::UCustomCharacterMovementComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	GravityScale = 2.8;
	JumpZVelocity = 900;
	AirControl = 0;
	AirControlBoostMultiplier = 0;
	AirControlBoostVelocityThreshold = 0;
	WalkingDirection = FVector2D::ZeroVector;
	PreviousWalkingDirection = FVector2D::ZeroVector;
}

void UCustomCharacterMovementComponent::CalcVelocity(const float DeltaTime, const float Friction, const bool bFluid, const float BrakingDeceleration)
{
	Super::CalcVelocity(DeltaTime, Friction, bFluid, BrakingDeceleration);

	// Air control for mouse movement to change direction mid-air after jumping
	if (IsMovingOnGround() && !WalkingDirection.IsZero()) {
		PreviousWalkingDirection = WalkingDirection;
	} else if (!IsMovingOnGround() && !PreviousWalkingDirection.IsZero()) {
		FVector VelocityDirection;

		if (PreviousWalkingDirection.X != 0 && PreviousWalkingDirection.Y != 0) {
			if (PreviousWalkingDirection.X > 0 && PreviousWalkingDirection.Y > 0) {
				// Forward and right
				VelocityDirection = UpdatedComponent->GetForwardVector() + UpdatedComponent->GetRightVector();
			} else if (PreviousWalkingDirection.X > 0 && PreviousWalkingDirection.Y < 0) {
				// Forward and left
				VelocityDirection = UpdatedComponent->GetForwardVector() - UpdatedComponent->GetRightVector();
			} else if (PreviousWalkingDirection.X < 0 && PreviousWalkingDirection.Y > 0) {
				// Back and right
				VelocityDirection = UpdatedComponent->GetRightVector() - UpdatedComponent->GetForwardVector();
			} else if (PreviousWalkingDirection.X < 0 && PreviousWalkingDirection.Y < 0) {
				// Back and left
				VelocityDirection = -(UpdatedComponent->GetForwardVector() + UpdatedComponent->GetRightVector());
			}

			VelocityDirection = UKismetMathLibrary::Normal(VelocityDirection / 2);
		} else if (PreviousWalkingDirection.X > 0) {
			// Forward
			VelocityDirection = UpdatedComponent->GetForwardVector();
		} else if (PreviousWalkingDirection.X < 0) {
			// Back
			VelocityDirection = -UpdatedComponent->GetForwardVector();
		} else if (PreviousWalkingDirection.Y > 0) {
			// Right
			VelocityDirection = UpdatedComponent->GetRightVector();
		} else if (PreviousWalkingDirection.Y < 0) {
			// Left
			VelocityDirection = -UpdatedComponent->GetRightVector();
		}

		Velocity -= Velocity - VelocityDirection * Velocity.Size();
	}
}
