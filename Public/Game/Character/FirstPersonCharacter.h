// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "FirstPersonCharacter.generated.h"


UCLASS()
class TESTLIGHT_API AFirstPersonCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	explicit AFirstPersonCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;

protected:
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

	UFUNCTION(Server, Unreliable)
	void Server_SetSprintSpeed(const float NewMaxWalkSpeed);
	void Server_SetSprintSpeed_Implementation(const float NewMaxWalkSpeed);

	// Sets the direction for custom movement component to follow the mouse while jumping
	UFUNCTION(Server, Unreliable)
	void Server_SetWalkingDirection(const float DirectionX, const float DirectionY);
	void Server_SetWalkingDirection_Implementation(const float DirectionX, const float DirectionY);
	
private:
	UPROPERTY()
	class UCustomCharacterMovementComponent* CustomCharacterMovementComponent;

	// FPS camera and mesh
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CharacterCameraComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* CharacterMeshComponent;

	// Max walking speed multiplied by this value
	UPROPERTY(EditAnywhere, Category = "Character Movement: Walking")
	float SprintSpeedMultiplier;
};