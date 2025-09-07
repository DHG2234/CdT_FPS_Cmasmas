// Fill out your copyright notice in the Description page of Project Settings.


#include "Checkpoint.h"
#include "Components/CapsuleComponent.h"
#include "FTPSCharacter.h"

ACheckpoint::ACheckpoint()
{
	PrimaryActorTick.bCanEverTick = false;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	RootComponent = CapsuleComponent;

	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CapsuleComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	CapsuleComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	RespawnOffset = FVector(0.f, 0.f, 50.f);
}

void ACheckpoint::BeginPlay()
{
	Super::BeginPlay();
	CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &ACheckpoint::OnCheckpointOverlap);
}

void ACheckpoint::OnCheckpointOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AFTPSCharacter* Player = Cast<AFTPSCharacter>(OtherActor);
	if (IsValid(Player))
	{
		Player->InitialPosition = GetActorLocation() + RespawnOffset;
		Player->InitialRotation = GetActorRotation();
	}
}