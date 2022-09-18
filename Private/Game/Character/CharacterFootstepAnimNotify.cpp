// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Character/CharacterFootstepAnimNotify.h"
#include "Game/Character/FirstPersonCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

UCharacterFootstepAnimNotify::UCharacterFootstepAnimNotify()
{
	static ConstructorHelpers::FObjectFinder<USoundCue> FootstepSoundFile(TEXT("/Game/Character/Sounds/SC_Footstep"));
	FootstepSound = FootstepSoundFile.Object;
}

void UCharacterFootstepAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	AFirstPersonCharacter* Character = Cast<AFirstPersonCharacter>(MeshComp->GetOwner());
	if (Character == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("Cannot get Character in CharacterFootstepAnimNotify"));
		return;
	}

	// I do not know why but this event is fired for all clients
	if (Character->IsLocallyControlled()) {
		Character->Server_PlayPlayerSound(FootstepSound);
	}
}
