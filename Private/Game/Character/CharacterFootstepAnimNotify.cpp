// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Character/CharacterFootstepAnimNotify.h"
#include "Game/Character/FirstPersonCharacter.h"
#include "Kismet/GameplayStatics.h"

void UCharacterFootstepAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (FootstepSound == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("Cannot get FootstepSound in CharacterFootstepAnimNotify"));
		return;
	}

	const AFirstPersonCharacter* Character = Cast<AFirstPersonCharacter>(MeshComp->GetOwner());
	if (Character == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("Cannot get Character in CharacterFootstepAnimNotify"));
		return;
	}

	if (Character->IsLocallyControlled()) {
		// Play the footsteps of the current player as 2d 
		UGameplayStatics::PlaySound2D(
			Character->GetWorld(),
			FootstepSound
		);
	} else {
		// Play the footsteps of other players at location 
		UGameplayStatics::PlaySoundAtLocation(
			Character->GetWorld(),
			FootstepSound,
			Character->GetActorLocation(),
			Character->GetActorRotation()
		);
	}
}
