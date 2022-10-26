// Copyright Epic Games, Inc. All Rights Reserved.

#include "Game/Character/FirstPersonCharacter.h"
#include "Game/Character/CustomCharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Net/UnrealNetwork.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFirstPersonCharacter::AFirstPersonCharacter(const FObjectInitializer& ObjectInitializer):
	Super(ObjectInitializer.SetDefaultSubobjectClass<UCustomCharacterMovementComponent>(CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

	// Set default sprint speed multiplier
	SprintSpeedMultiplier = 1.3f;

	// Create a first person camera component.
	CharacterCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	if (!CharacterCameraComponent) {
		UE_LOG(LogTemp, Error, TEXT("Cannot create FPSCameraComponent in FirstPersonCharacter"));
		return;
	}

	// Attach the camera component to our capsule component.
	CharacterCameraComponent->SetupAttachment(CastChecked<USceneComponent, UCapsuleComponent>(GetCapsuleComponent()));

	// Position the camera slightly above the eyes.
	CharacterCameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, BaseEyeHeight));

	// Enable the pawn to control camera rotation.
	CharacterCameraComponent->bUsePawnControlRotation = true;

	// Create a first person mesh component for the owning player.
	CharacterMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	if (!CharacterCameraComponent) {
		UE_LOG(LogTemp, Error, TEXT("Cannot create FPSMesh in FirstPersonCharacter"));
		return;
	}

	// Only the owning player sees this mesh.
	CharacterMeshComponent->SetOnlyOwnerSee(true);

	// Attach the FPS mesh to the FPS camera.
	CharacterMeshComponent->SetupAttachment(CharacterCameraComponent);

	// Disable some environmental shadows to preserve the illusion of having a single mesh.
	CharacterMeshComponent->bCastDynamicShadow = false;
	CharacterMeshComponent->CastShadow = false;

	// The owning player doesn't see the regular (third-person) body mesh.
	GetMesh()->SetOwnerNoSee(true);
}

// Called to bind functionality to input
void AFirstPersonCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AFirstPersonCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFirstPersonCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &AFirstPersonCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &AFirstPersonCharacter::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AFirstPersonCharacter::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AFirstPersonCharacter::StopJump);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AFirstPersonCharacter::StartSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AFirstPersonCharacter::StopSprint);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AFirstPersonCharacter::StartCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AFirstPersonCharacter::StopCrouch);
}

void AFirstPersonCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	CustomCharacterMovementComponent = Cast<UCustomCharacterMovementComponent>(Super::GetMovementComponent());
}

void AFirstPersonCharacter::MoveForward(const float Value)
{
	Server_SetWalkingDirection_Implementation(Value, CustomCharacterMovementComponent->WalkingDirection.Y);

	// If you're a server, you don't need to set SetWalkingDirection via server
	if (!HasAuthority()) {
		Server_SetWalkingDirection(Value, CustomCharacterMovementComponent->WalkingDirection.Y);
	}

	const FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction, Value);
}

void AFirstPersonCharacter::MoveRight(const float Value)
{
	Server_SetWalkingDirection_Implementation(CustomCharacterMovementComponent->WalkingDirection.X, Value);

	// If you're a server, you don't need to set SetWalkingDirection via server
	if (!HasAuthority()) {
		Server_SetWalkingDirection(CustomCharacterMovementComponent->WalkingDirection.X, Value);
	}

	const FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, Value);
}

void AFirstPersonCharacter::StartJump()
{
	bPressedJump = true;
}

void AFirstPersonCharacter::StopJump()
{
	bPressedJump = false;
}

void AFirstPersonCharacter::StartCrouch()
{
	Crouch();
}

void AFirstPersonCharacter::StopCrouch()
{
	UnCrouch();
}

void AFirstPersonCharacter::StartSprint()
{
	const float NewMaxWalkSpeed = GetCharacterMovement()->MaxWalkSpeed * SprintSpeedMultiplier;
	Server_SetSprintSpeed_Implementation(NewMaxWalkSpeed);

	if (!HasAuthority()) {
		Server_SetSprintSpeed(NewMaxWalkSpeed);
	}
}

void AFirstPersonCharacter::StopSprint()
{
	const float NewMaxWalkSpeed = GetCharacterMovement()->MaxWalkSpeed / SprintSpeedMultiplier;
	Server_SetSprintSpeed_Implementation(NewMaxWalkSpeed);

	if (!HasAuthority()) {
		Server_SetSprintSpeed(NewMaxWalkSpeed);
	}
}

void AFirstPersonCharacter::Server_SetSprintSpeed_Implementation(const float NewMaxWalkSpeed)
{
	CustomCharacterMovementComponent->MaxWalkSpeed = NewMaxWalkSpeed;
}

void AFirstPersonCharacter::Server_SetWalkingDirection_Implementation(const float DirectionX, const float DirectionY)
{
	CustomCharacterMovementComponent->WalkingDirection = FVector2D(DirectionX, DirectionY);
}
