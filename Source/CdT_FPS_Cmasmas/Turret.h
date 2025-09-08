// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Turret.generated.h"

class UStaticMeshComponent;
class USceneComponent;
class UParticleSystem;

UCLASS()
class CDT_FPS_CMASMAS_API ATurret : public AActor
{
	GENERATED_BODY()

public:
	ATurret();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* StaticMesh;

	// --- Muzzle ---
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* Muzzle;

	UPROPERTY(EditDefaultsOnly, Category = "FX")
	UParticleSystem* MuzzleEffect;

	// --- Bullet Class ---
	UPROPERTY(EditAnywhere, Category = "Turret")
	TSubclassOf<class ATurretBullet> BulletClass;

	// --- Fire Delay ---
	FTimerHandle TimerHandle_Fire;

	UPROPERTY(EditAnywhere, Category = "Turret")
	float FireRate;

	UPROPERTY(EditAnywhere, Category = "Turret")
	float TimeFireDelay;

	UPROPERTY(EditAnywhere, Category = "Turret")
	int32 BulletCount;

	UPROPERTY(EditAnywhere, Category = "Turret")
	bool bIsInfinite;
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
	bool bIsActive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret")
	bool bIsRotating;

public:
	virtual void Tick(float DeltaTime) override;

	void StartFire();
	void StopFire();
	void Fire();
};
