// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorKey.h"
#include "Components/StaticMeshComponent.h"
#include "FTPSCharacter.h"

ADoorKey::ADoorKey()
{
	KeyMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("KeyMeshComponent"));
	KeyMeshComponent->SetupAttachment(RootComponent);
	KeyMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	KeyTag = "KeyA";
}

void ADoorKey::Pickup(AFTPSCharacter* PickupActor)
{
	Super::Pickup(PickupActor);
	PickupActor->AddKey(KeyTag);
	Destroy();

}