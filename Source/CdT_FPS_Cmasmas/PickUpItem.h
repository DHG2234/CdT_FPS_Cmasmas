// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "PickUpItem.generated.h"

UENUM(BlueprintType)
enum class EPickUpItemType : uint8
{
    Item_Health UMETA(DisplayName = "Health"),
    Item_Ammo   UMETA(DisplayName = "Ammo"),
    Item_Respawn UMETA(DisplayName = "Respawn")
};

UCLASS()
class CDT_FPS_CMASMAS_API APickUpItem : public AItem
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PickUp")
    EPickUpItemType ItemType;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PickUp")
    int32 Amount;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
    UStaticMeshComponent* Mesh;

public:
    APickUpItem();

protected:
    virtual void BeginPlay() override;

    virtual void Pickup(AFTPSCharacter* PickupActor) override;
};