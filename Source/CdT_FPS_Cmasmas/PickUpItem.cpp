// Fill out your copyright notice in the Description page of Project Settings.

#include "PickUpItem.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "FTPSCharacter.h"
#include "Weapon.h"
#include "HealthComponent.h"

APickUpItem::APickUpItem()
{
    // Ya heredamos MainColliderComponent de AItem (RootComponent)
    // Solo añadimos un mesh visible
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    Mesh->SetupAttachment(MainColliderComponent);
}

void APickUpItem::BeginPlay()
{
    Super::BeginPlay();
}

void APickUpItem::Pickup(AFTPSCharacter* PickupActor)
{
    if (!IsValid(PickupActor)) return;

    switch (ItemType)
    {
    case EPickUpItemType::Item_Health:
        if (IsValid(PickupActor->GetHealthComponent()) &&
            PickupActor->GetHealthComponent()->GetHealth() < PickupActor->GetHealthComponent()->GetMaxHealth())
        {
            PickupActor->GetHealthComponent()->AddHealth(Amount);
            Destroy();
        }
        break;

    case EPickUpItemType::Item_Ammo:
        if (IsValid(PickupActor->GetCurrentWeapon()) &&
            PickupActor->GetCurrentWeapon()->GetCurrentAmmo() < PickupActor->GetCurrentWeapon()->GetMaxAmmo())
        {
            PickupActor->GetCurrentWeapon()->AddAmmo(Amount);
            Destroy();
        }
        break;

    case EPickUpItemType::Item_Respawn:
        if (PickupActor->GetRespawns() < PickupActor->GetMaxRespawns())
        {
            PickupActor->AddRespawn(Amount);
            Destroy();
        }
        break;

    default:
        break;
    }
}