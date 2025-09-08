// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TurretBullet.generated.h"

class USphereComponent;
class UParticleSystemComponent;

UCLASS()
class CDT_FPS_CMASMAS_API ATurretBullet : public AActor
{
	GENERATED_BODY()

public:
	ATurretBullet();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* SphereCollision;

	// --- Projectile Particle ---
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UParticleSystemComponent* BulletTrail;

	// --- Projectile Movement Speed ---
	UPROPERTY(EditAnywhere, Category = "Bullet")
	float SpeedBullet;

	// --- Damage ---
	UPROPERTY(EditAnywhere, Category = "Bullet")
	float Damage;

	// --- Hit Particle ---
	UPROPERTY(EditDefaultsOnly, Category = "FX")
	UParticleSystem* HitWorldParticle;

	UPROPERTY(EditDefaultsOnly, Category = "FX")
	UParticleSystem* HitPlayerParticle;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		FVector NormalImpulse, const FHitResult& Hit);
};