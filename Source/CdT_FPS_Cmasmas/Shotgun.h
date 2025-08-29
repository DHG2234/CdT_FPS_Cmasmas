// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Shotgun.generated.h"

class AProjectile;

UCLASS()
class CDT_FPS_CMASMAS_API AShotgun : public AWeapon
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
	FName ProjectileSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
	TSubclassOf<AProjectile> ProjectileClass;

public:

	AShotgun();

protected:

	virtual void StartAction() override;

	virtual void StopAction() override;
};