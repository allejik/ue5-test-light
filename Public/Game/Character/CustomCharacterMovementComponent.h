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

public:
	// Custom character move class
	class FSavedMove_FirstPersonCharacter : public FSavedMove_Character
	{
	public:
		typedef FSavedMove_Character Super;

		// Returns true if this move can be combined with NewMove for replication without changing any behavior
		virtual bool CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* Character, float MaxDelta) const override;

		// Clear saved move properties, so it can be re-used
		virtual void Clear() override;

		// Returns a byte containing encoded special movement information (jumping, crouching, etc.)
		virtual uint8 GetCompressedFlags() const override;

		// Called to set up this saved move (when initially created) to make a predictive correction
		virtual void SetMoveFor(ACharacter* Character, float InDeltaTime, FVector const& NewAccel, FNetworkPredictionData_Client_Character & ClientData) override;

		// Called before ClientUpdatePosition uses this SavedMove to make a predictive correction
		virtual void PrepMoveFor(ACharacter* Character) override;

		// Walk Speed Update
		uint8 bSavedRequestMaxWalkSpeedChange : 1;

		// Dodge
		FVector SavedMoveDirection;
		uint8 bSavedWantsToDodge : 1;
	};

	// Custom prediction data class
	class FNetworkPredictionData_Client_FirstPersonCharacter : public FNetworkPredictionData_Client_Character
	{
	public:
		explicit FNetworkPredictionData_Client_FirstPersonCharacter(const UCharacterMovementComponent& ClientMovement);
		typedef FNetworkPredictionData_Client_Character Super;

		// Allocate a new saved move. Subclasses should override this if they want to use a custom move class
		virtual FSavedMovePtr AllocateNewMove() override;
	};

	explicit UCustomCharacterMovementComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// Get prediction data for a client game. Should not be used if not running as a client. Allocates the data on demand and can be overridden to allocate a custom override if desired
	virtual FNetworkPredictionData_Client* GetPredictionData_Client() const override;

	// Handle a pending launch during an update. Returns true if the launch was triggered
	virtual bool HandlePendingLaunch() override;

	// Updates Velocity and Acceleration based on the current state
	virtual void CalcVelocity(float DeltaTime, float Friction, bool bFluid, float BrakingDeceleration) override;

protected:
	// Unpack compressed flags from a saved move and set state accordingly
	virtual void UpdateFromCompressedFlags(uint8 Flags) override;

	// Event triggered at the end of a movement update. If scoped movement updates are enabled (bEnableScopedMovementUpdates), this is within such a scope
	virtual void OnMovementUpdated(float DeltaTime, const FVector& OldLocation, const FVector& OldVelocity) override;

	UFUNCTION(Unreliable, Server, WithValidation)
	void ServerSetMaxWalkSpeed(const float NewMaxWalkSpeed);

	UFUNCTION(Unreliable, Server)
	void ServerMoveDirection(const FVector& MoveDir);

private:
	int32 LastYawOnGround;
	
public:
	uint8 bRequestMaxWalkSpeedChange : 1;
	uint8 bWantsToDodge : 1;
	float CharacterNewMaxWalkSpeed;
	FVector MoveDirection;

	UPROPERTY(EditAnywhere, Category = "Character Movement: Walking")
	float SprintSpeedMultiplier;

	UPROPERTY(EditAnywhere, Category = "Character Movement: Walking")
	float DodgeStrength;

	// Increases/Decreases character's speed based MaxWalkSpeed*SprintSpeedMultiplier
	void StartSprint();
	void StopSprint();

	// Pushes character in the direction of movement while on the ground
	void DoDodge();
};
