// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "FirstPersonCharacter.generated.h"


UCLASS()
class TESTLIGHT_API AFirstPersonCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	explicit AFirstPersonCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	FORCEINLINE class UCustomCharacterMovementComponent* GetCustomCharacterMovementComponent() const { return CustomCharacterMovementComponent; }

	// Movement
	UFUNCTION()
	void MoveForward(float Value);
	UFUNCTION()
	void MoveRight(float Value);
	UFUNCTION()
	void StartJump();
	UFUNCTION()
	void StopJump();
	UFUNCTION()
	void StartSprint();
	UFUNCTION()
	void StopSprint();
	UFUNCTION()
	void StartCrouch();
	UFUNCTION()
	void StopCrouch();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetSprintSpeed(const float NewMaxWalkSpeed);
	bool Server_SetSprintSpeed_Validate(const float NewMaxWalkSpeed);
	void Server_SetSprintSpeed_Implementation(const float NewMaxWalkSpeed);

	// Sets the direction for custom movement component to follow the mouse while jumping
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetWalkingDirection(const float DirectionX, const float DirectionY);
	bool Server_SetWalkingDirection_Validate(const float DirectionX, const float DirectionY);
	void Server_SetWalkingDirection_Implementation(const float DirectionX, const float DirectionY);

	// Plays player sound for player and other players
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayPlayerSound(USoundBase* Sound);
	void Multicast_PlayPlayerSound_Implementation(USoundBase* Sound);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_PlayPlayerSound(USoundBase* Sound);
	void Server_PlayPlayerSound_Implementation(USoundBase* Sound);
	bool Server_PlayPlayerSound_Validate(USoundBase* Sound);

	// FPS camera and mesh
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CharacterCameraComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* CharacterMeshComponent;

	// Max walking speed multiplied by this value
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement: Walking")
	float SprintSpeedMultiplier;

private:
	UPROPERTY()
	UCustomCharacterMovementComponent* CustomCharacterMovementComponent;

// Online session
public:
	IOnlineSessionPtr OnlineSessionInterface; 

protected:
	UFUNCTION(BlueprintCallable)
	void CreateGameSession();
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
private:
	FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;
};