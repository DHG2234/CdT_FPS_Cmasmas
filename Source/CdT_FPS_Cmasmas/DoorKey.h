// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "DoorKey.generated.h"

class UStaticMeshComponent;

UCLASS()
class CDT_FPS_CMASMAS_API ADoorKey : public AItem
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* KeyMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Key")
	FName KeyTag;

public:

	ADoorKey();
	UFUNCTION(BlueprintCallable, Category = "Key")
	FName GetKeyTag() const { return KeyTag; }

protected:

	virtual void Pickup(AFTPSCharacter* PickupActor) override;

};
