// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "FPSCharacter.generated.h"


UCLASS()
class TESTLIGHT_API AFPSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	explicit AFPSCharacter(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "Movement")
	FORCEINLINE class UCustomCharacterMovementComponent* GetCustomCharacterMovementComponent() const { return CustomCharacterMovementComponent; }
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void AddPlayerHUD();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PostInitializeComponents() override;

	// Handles input for moving forward and backward.
	UFUNCTION()
	void MoveForward(float Value);

	// Handles input for moving right and left.
	UFUNCTION()
	void MoveRight(float Value);

	UFUNCTION()
	void StartJump();

	UFUNCTION()
	void StopJump();

	void StartSprint();
	void StopSprint();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetSprintSpeed(const float NewMaxWalkSpeed);
	bool Server_SetSprintSpeed_Validate(const float NewMaxWalkSpeed);
	void Server_SetSprintSpeed_Implementation(const float NewMaxWalkSpeed);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetWalkingDirection(const float DirectionX, const float DirectionY);
	bool Server_SetWalkingDirection_Validate(const float DirectionX, const float DirectionY);
	void Server_SetWalkingDirection_Implementation(const float DirectionX, const float DirectionY);

	// FPS camera
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* FPSCameraComponent;

	// First-person mesh (arms), visible only to the owning player.
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* FPSMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character Movement: Walking")
	float SprintSpeedMultiplier;

	// Player HUD
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UPlayerHUD> PlayerHUDClass;

	UPROPERTY()
	UPlayerHUD* PlayerHUD;
private:
	UPROPERTY()
	UCustomCharacterMovementComponent* CustomCharacterMovementComponent;
};
