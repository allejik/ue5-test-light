// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Character/CharacterFootstepAnimNotify.h"
#include "Game/Character/FirstPersonCharacter.h"

void UCharacterFootstepAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	AFirstPersonCharacter* Character = Cast<AFirstPersonCharacter>(MeshComp->GetOwner());
	if (Character == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("Cannot get Character in CharacterFootstepAnimNotify"));
		return;
	}

	Character->PlayerFootstepSound();
}
