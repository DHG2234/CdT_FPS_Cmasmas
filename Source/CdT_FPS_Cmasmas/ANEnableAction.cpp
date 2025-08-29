// Fill out your copyright notice in the Description page of Project Settings.


#include "ANEnableAction.h"
#include "FTPSCharacter.h"

void UANEnableAction::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AActor* CharacterActor = MeshComp->GetOwner();
	if (IsValid(CharacterActor))
	{
		AFTPSCharacter* Character = Cast<AFTPSCharacter>(CharacterActor);
		if (IsValid(Character))
		{
			Character->SetActionsState(false);
			UE_LOG(LogTemp, Warning, TEXT("Entre"));
		}
	}
}
