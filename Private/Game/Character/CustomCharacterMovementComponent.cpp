// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Character/CustomCharacterMovementComponent.h"
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
	DodgeStrength = 10000.f;
	SprintSpeedMultiplier = 1.3f;
	LastYawOnGround = 0;
}

void UCustomCharacterMovementComponent::OnMovementUpdated(const float DeltaTime, const FVector& OldLocation, const FVector& OldVelocity)
{
	Super::OnMovementUpdated(DeltaTime, OldLocation, OldVelocity);

	if (!CharacterOwner) {
		return;
	}

	// Updates character max walk speed
	if (bRequestMaxWalkSpeedChange) {
		// Sets false so it executes only once
		bRequestMaxWalkSpeedChange = false;
		MaxWalkSpeed = CharacterNewMaxWalkSpeed;
	}

	// Adds dodge movement
	if (bWantsToDodge) {
		// Sets false so it executes only once
		bWantsToDodge = false;

		// Only dodge if on the ground (in the air causes problems trying to get the two modes to line up due to friction)
		if (IsMovingOnGround()) {
			MoveDirection.Normalize();
			FVector DodgeVelocity = MoveDirection * DodgeStrength;
			DodgeVelocity.Z = 0.0f;

			Launch(DodgeVelocity);
		}
	}
}

void UCustomCharacterMovementComponent::UpdateFromCompressedFlags(const uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);

	bRequestMaxWalkSpeedChange = (Flags&FSavedMove_Character::FLAG_Custom_0) != 0;
	bWantsToDodge = (Flags&FSavedMove_Character::FLAG_Custom_1) != 0;
}

FNetworkPredictionData_Client* UCustomCharacterMovementComponent::GetPredictionData_Client() const
{
	if (PawnOwner == nullptr || PawnOwner->GetLocalRole() >= ROLE_Authority) {
		return ClientPredictionData;
	}

	if (!ClientPredictionData) {
		UCustomCharacterMovementComponent* MutableThis = const_cast<UCustomCharacterMovementComponent*>(this);

		MutableThis->ClientPredictionData = new FNetworkPredictionData_Client_FirstPersonCharacter(*this);
		MutableThis->ClientPredictionData->MaxSmoothNetUpdateDist = 92.f;
		MutableThis->ClientPredictionData->NoSmoothNetUpdateDist = 140.f;
	}

	return ClientPredictionData;
}

void UCustomCharacterMovementComponent::FSavedMove_FirstPersonCharacter::Clear()
{
	Super::Clear();

	bSavedRequestMaxWalkSpeedChange = false;
	bSavedWantsToDodge = false;
	SavedMoveDirection = FVector::ZeroVector;
}

uint8 UCustomCharacterMovementComponent::FSavedMove_FirstPersonCharacter::GetCompressedFlags() const
{
	uint8 Result = Super::GetCompressedFlags();

	if (bSavedRequestMaxWalkSpeedChange) {
		Result |= FLAG_Custom_0;
	}

	if (bSavedWantsToDodge) {
		Result |= FLAG_Custom_1;
	}

	return Result;
}

bool UCustomCharacterMovementComponent::FSavedMove_FirstPersonCharacter::CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* Character, float MaxDelta) const
{
	if (bSavedRequestMaxWalkSpeedChange != ((FSavedMove_FirstPersonCharacter*)&NewMove)->bSavedRequestMaxWalkSpeedChange) {
		return false;
	}
	if (bSavedWantsToDodge != ((FSavedMove_FirstPersonCharacter*)&NewMove)->bSavedWantsToDodge) {
		return false;
	}
	if (SavedMoveDirection != ((FSavedMove_FirstPersonCharacter*)&NewMove)->SavedMoveDirection) {
		return false;
	}

	return Super::CanCombineWith(NewMove, Character, MaxDelta);
}

void UCustomCharacterMovementComponent::FSavedMove_FirstPersonCharacter::SetMoveFor(ACharacter* Character, const float InDeltaTime, FVector const& NewAccel, FNetworkPredictionData_Client_Character & ClientData)
{
	Super::SetMoveFor(Character, InDeltaTime, NewAccel, ClientData);

	const UCustomCharacterMovementComponent* CharacterMovement = Cast<UCustomCharacterMovementComponent>(Character->GetCharacterMovement());
	if (CharacterMovement) {
		bSavedRequestMaxWalkSpeedChange = CharacterMovement->bRequestMaxWalkSpeedChange;
		bSavedWantsToDodge = CharacterMovement->bWantsToDodge;
		SavedMoveDirection = CharacterMovement->MoveDirection;
	}
}

void UCustomCharacterMovementComponent::FSavedMove_FirstPersonCharacter::PrepMoveFor(ACharacter* Character)
{
	Super::PrepMoveFor(Character);

	UCustomCharacterMovementComponent* CharacterMovement = Cast<UCustomCharacterMovementComponent>(Character->GetCharacterMovement());
	if (CharacterMovement) {
		CharacterMovement->MoveDirection = SavedMoveDirection;
	}
}

UCustomCharacterMovementComponent::FNetworkPredictionData_Client_FirstPersonCharacter::FNetworkPredictionData_Client_FirstPersonCharacter(const UCharacterMovementComponent& ClientMovement)
	: Super(ClientMovement)
{}

FSavedMovePtr UCustomCharacterMovementComponent::FNetworkPredictionData_Client_FirstPersonCharacter::AllocateNewMove()
{
	return MakeShared<FSavedMove_FirstPersonCharacter>();
}

bool UCustomCharacterMovementComponent::HandlePendingLaunch()
{
	if (!PendingLaunchVelocity.IsZero() && HasValidData()) {
		Velocity = PendingLaunchVelocity;
		// Remmed out so our dodge move won't play the falling animation.
		// SetMovementMode(MOVE_Falling);
		PendingLaunchVelocity = FVector::ZeroVector;
		bForceNextFloorCheck = true;
		return true;
	}

	return false;
}

bool UCustomCharacterMovementComponent::ServerSetMaxWalkSpeed_Validate(const float NewMaxWalkSpeed)
{
	return NewMaxWalkSpeed >= 0.f && NewMaxWalkSpeed <= 2000.f;
}

void UCustomCharacterMovementComponent::ServerSetMaxWalkSpeed_Implementation(const float NewMaxWalkSpeed)
{
	CharacterNewMaxWalkSpeed = NewMaxWalkSpeed;
}

void UCustomCharacterMovementComponent::ServerMoveDirection_Implementation(const FVector& MoveDir)
{
	MoveDirection = MoveDir;
}

void UCustomCharacterMovementComponent::StartSprint()
{
	if (PawnOwner->IsLocallyControlled()) {
		const float NewMaxWalkSpeed = MaxWalkSpeed * SprintSpeedMultiplier;
		CharacterNewMaxWalkSpeed = NewMaxWalkSpeed;
		ServerSetMaxWalkSpeed(NewMaxWalkSpeed);
	}

	bRequestMaxWalkSpeedChange = true;
}

void UCustomCharacterMovementComponent::StopSprint()
{
	if (PawnOwner->IsLocallyControlled()) {
		const float NewMaxWalkSpeed = MaxWalkSpeed / SprintSpeedMultiplier;
		CharacterNewMaxWalkSpeed = NewMaxWalkSpeed;
		ServerSetMaxWalkSpeed(NewMaxWalkSpeed);
	}

	bRequestMaxWalkSpeedChange = true;
}

void UCustomCharacterMovementComponent::DoDodge()
{
	if (PawnOwner->IsLocallyControlled()) {
		MoveDirection = PawnOwner->GetLastMovementInputVector();
		ServerMoveDirection(MoveDirection);
	}

	bWantsToDodge = true;
}

void UCustomCharacterMovementComponent::CalcVelocity(const float DeltaTime, const float Friction, const bool bFluid, const float BrakingDeceleration)
{
	Super::CalcVelocity(DeltaTime, Friction, bFluid, BrakingDeceleration);

	if (IsMovingOnGround() && Velocity.Size() > 0) {
		const FVector NewDirection = UKismetMathLibrary::InverseTransformDirection(GetActorTransform(), Velocity);
		LastYawOnGround = FMath::CeilToInt(FRotationMatrix::MakeFromX(NewDirection).Rotator().Yaw);
	}

	// Air control for mouse movement to change direction mid-air after jumping
	if (!IsMovingOnGround()) {
		FVector VelocityDirection;
		const bool IsMovingForward = UKismetMathLibrary::InRange_IntInt(LastYawOnGround, -22, 0) || UKismetMathLibrary::InRange_IntInt(LastYawOnGround, 0, 22);
		const bool IsMovingForwardRight = UKismetMathLibrary::InRange_IntInt(LastYawOnGround, 23, 67);
		const bool IsMovingRight = UKismetMathLibrary::InRange_IntInt(LastYawOnGround, 68, 112);
		const bool IsMovingBackwardRight = UKismetMathLibrary::InRange_IntInt(LastYawOnGround, 113, 157);
		const bool IsMovingBackward = UKismetMathLibrary::InRange_IntInt(LastYawOnGround, -180, -158) || UKismetMathLibrary::InRange_IntInt(LastYawOnGround, 158, 180);
		const bool IsMovingBackwardLeft = UKismetMathLibrary::InRange_IntInt(LastYawOnGround, -157, -113);
		const bool IsMovingLeft = UKismetMathLibrary::InRange_IntInt(LastYawOnGround, -112, -68);
		const bool IsMovingForwardLeft = UKismetMathLibrary::InRange_IntInt(LastYawOnGround, -67, -23);

		if (IsMovingForward) {
			VelocityDirection = UpdatedComponent->GetForwardVector();
		} else if (IsMovingBackward) {
			VelocityDirection = -UpdatedComponent->GetForwardVector();
		} else if (IsMovingRight) {
			VelocityDirection = UpdatedComponent->GetRightVector();
		} else if (IsMovingLeft) {
			VelocityDirection = -UpdatedComponent->GetRightVector();
		} else {
			if (IsMovingForwardRight) {
				VelocityDirection = UpdatedComponent->GetForwardVector() + UpdatedComponent->GetRightVector();
			} else if (IsMovingForwardLeft) {
				VelocityDirection = UpdatedComponent->GetForwardVector() - UpdatedComponent->GetRightVector();
			} else if (IsMovingBackwardRight) {
				VelocityDirection = UpdatedComponent->GetRightVector() - UpdatedComponent->GetForwardVector();
			} else if (IsMovingBackwardLeft) {
				VelocityDirection = -(UpdatedComponent->GetForwardVector() + UpdatedComponent->GetRightVector());
			}
			
			VelocityDirection = UKismetMathLibrary::Normal(VelocityDirection / 2);
		}

		Velocity -= Velocity - VelocityDirection * Velocity.Size();
	}
}