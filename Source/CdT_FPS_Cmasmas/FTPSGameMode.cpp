// Fill out your copyright notice in the Description page of Project Settings.


#include "FTPSGameMode.h"
#include "FTPSCharacter.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Components/CapsuleComponent.h"

void AFTPSGameMode::GameOver(AFTPSCharacter* Character)
{
	Character->GetMovementComponent()->StopMovementImmediately();
	Character->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Character->DetachFromControllerPendingDestroy();
	Character->SetLifeSpan(5.0f);

	BP_GameOver(Character);
}
