// Copyright Epic Games, Inc. All Rights Reserved.

#include "Game/FPSCharacter.h"
#include "Game/CustomCharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Net/UnrealNetwork.h"
#include "Blueprint/UserWidget.h"

// Sets default values
AFPSCharacter::AFPSCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCustomCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set default sprint speed multiplier
	SprintSpeedMultiplier = 1.5f;

	// Create a first person camera component.
	FPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	check(FPSCameraComponent != nullptr);

	// Attach the camera component to our capsule component.
	FPSCameraComponent->SetupAttachment(CastChecked<USceneComponent, UCapsuleComponent>(GetCapsuleComponent()));

	// Position the camera slightly above the eyes.
	FPSCameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f + BaseEyeHeight));

	// Enable the pawn to control camera rotation.
	FPSCameraComponent->bUsePawnControlRotation = true;

	// Create a first person mesh component for the owning player.
	FPSMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	check(FPSMesh != nullptr);

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

// Called when the game starts or when spawned
void AFPSCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AFPSCharacter::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AFPSCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPSCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &AFPSCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &AFPSCharacter::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AFPSCharacter::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AFPSCharacter::StopJump);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AFPSCharacter::StartSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AFPSCharacter::StopSprint);
}

void AFPSCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	CustomCharacterMovementComponent = Cast<UCustomCharacterMovementComponent>(Super::GetMovementComponent());
}

void AFPSCharacter::MoveForward(const float Value)
{
	Server_SetWalkingDirection_Implementation(Value, CustomCharacterMovementComponent->WalkingDirection.Y);
	if (!HasAuthority()) {
		Server_SetWalkingDirection(Value, CustomCharacterMovementComponent->WalkingDirection.Y);
	}

	const FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction, Value);
}

void AFPSCharacter::MoveRight(const float Value)
{
	Server_SetWalkingDirection_Implementation(CustomCharacterMovementComponent->WalkingDirection.X, Value);
	if (!HasAuthority()) {
		Server_SetWalkingDirection(CustomCharacterMovementComponent->WalkingDirection.X, Value);
	}

	const FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, Value);
}

void AFPSCharacter::StartJump()
{
	bPressedJump = true;
}

void AFPSCharacter::StopJump()
{
	bPressedJump = false;
}

void AFPSCharacter::StartSprint()
{
	const float NewMaxWalkSpeed = GetCharacterMovement()->MaxWalkSpeed * SprintSpeedMultiplier;
	Server_SetSprintSpeed_Implementation(NewMaxWalkSpeed);

	if (!HasAuthority()) {
		Server_SetSprintSpeed(NewMaxWalkSpeed);
	}
}

void AFPSCharacter::StopSprint()
{
	const float NewMaxWalkSpeed = GetCharacterMovement()->MaxWalkSpeed / SprintSpeedMultiplier;
	Server_SetSprintSpeed_Implementation(NewMaxWalkSpeed);

	if (!HasAuthority()) {
		Server_SetSprintSpeed(NewMaxWalkSpeed);
	}
}

void AFPSCharacter::Server_SetSprintSpeed_Implementation(const float NewMaxWalkSpeed)
{
	CustomCharacterMovementComponent->MaxWalkSpeed = NewMaxWalkSpeed;
}

bool AFPSCharacter::Server_SetSprintSpeed_Validate(const float NewMaxWalkSpeed)
{
	return true;
}

void AFPSCharacter::Server_SetWalkingDirection_Implementation(const float DirectionX, const float DirectionY)
{
	CustomCharacterMovementComponent->WalkingDirection = FVector2D(DirectionX, DirectionY);
}

bool AFPSCharacter::Server_SetWalkingDirection_Validate(const float DirectionX, const float DirectionY)
{
	return true;
}
