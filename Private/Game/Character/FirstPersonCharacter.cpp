// Copyright Epic Games, Inc. All Rights Reserved.

#include "Game/Character/FirstPersonCharacter.h"
#include "Game/Character/CustomCharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Net/UnrealNetwork.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFirstPersonCharacter::AFirstPersonCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCustomCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set default sprint speed multiplier
	SprintSpeedMultiplier = 1.3f;

	// Create a first person camera component.
	FPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	if (!FPSCameraComponent) {
		UE_LOG(LogTemp, Log, TEXT("Cannot create FPSCameraComponent in FirstPersonCharacter"));
		return;
	}

	// Attach the camera component to our capsule component.
	FPSCameraComponent->SetupAttachment(CastChecked<USceneComponent, UCapsuleComponent>(GetCapsuleComponent()));

	// Position the camera slightly above the eyes.
	FPSCameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, BaseEyeHeight));

	// Enable the pawn to control camera rotation.
	FPSCameraComponent->bUsePawnControlRotation = true;

	// Create a first person mesh component for the owning player.
	FPSMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	if (!FPSCameraComponent) {
		UE_LOG(LogTemp, Log, TEXT("Cannot create FPSMesh in FirstPersonCharacter"));
		return;
	}

	// Only the owning player sees this mesh.
	FPSMesh->SetOnlyOwnerSee(true);

	// Attach the FPS mesh to the FPS camera.
	FPSMesh->SetupAttachment(FPSCameraComponent);

	// Disable some environmental shadows to preserve the illusion of having a single mesh.
	FPSMesh->bCastDynamicShadow = false;
	FPSMesh->CastShadow = false;

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
}

void AFirstPersonCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	CustomCharacterMovementComponent = Cast<UCustomCharacterMovementComponent>(Super::GetMovementComponent());
}

void AFirstPersonCharacter::MoveForward(const float Value)
{
	Server_SetWalkingDirection_Implementation(Value, CustomCharacterMovementComponent->WalkingDirection.Y);
	if (!HasAuthority()) {
		Server_SetWalkingDirection(Value, CustomCharacterMovementComponent->WalkingDirection.Y);
	}

	const FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction, Value);
}

void AFirstPersonCharacter::MoveRight(const float Value)
{
	Server_SetWalkingDirection_Implementation(CustomCharacterMovementComponent->WalkingDirection.X, Value);
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

bool AFirstPersonCharacter::Server_SetSprintSpeed_Validate(const float NewMaxWalkSpeed)
{
	return true;
}

void AFirstPersonCharacter::Server_SetWalkingDirection_Implementation(const float DirectionX, const float DirectionY)
{
	CustomCharacterMovementComponent->WalkingDirection = FVector2D(DirectionX, DirectionY);
}

bool AFirstPersonCharacter::Server_SetWalkingDirection_Validate(const float DirectionX, const float DirectionY)
{
	return true;
}

void AFirstPersonCharacter::PlayerFootstepSound()
{
	if (IsLocallyControlled()) {
		UE_LOG(LogTemp, Warning, TEXT("sound at location: %s"), *FString(GetActorLocation().ToString()));
		UGameplayStatics::PlaySoundAtLocation(
			GetWorld(),
			FootstepSound,
			GetActorLocation()
		);
	}
}
