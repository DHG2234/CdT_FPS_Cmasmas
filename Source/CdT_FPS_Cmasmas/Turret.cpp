// Fill out your copyright notice in the Description page of Project Settings.

#include "Turret.h"
#include "TurretBullet.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Engine/World.h"
#include "TimerManager.h"

ATurret::ATurret()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMesh;

	Muzzle = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle"));
	Muzzle->SetupAttachment(StaticMesh);

	FireRate = 0.2f;
	TimeFireDelay = 2.0f;
	BulletCount = 5;
	bIsInfinite = false;
	bIsActive = true;
	bIsRotating = false;
}

void ATurret::BeginPlay()
{
	Super::BeginPlay();

	if (bIsActive)
	{
		StartFire();
	}
}

void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsRotating)
	{
		AddActorLocalRotation(FRotator(0.f, 60.f * DeltaTime, 0.f));
	}
}

void ATurret::StartFire()
{
	if (bIsInfinite)
	{
		GetWorldTimerManager().SetTimer(TimerHandle_Fire, this, &ATurret::Fire, FireRate, true, 0.0f);
	}
	else
	{
		// Disparo controlado por delay
		GetWorldTimerManager().SetTimer(TimerHandle_Fire, this, &ATurret::Fire, FireRate, true, TimeFireDelay);
	}
}

void ATurret::StopFire()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_Fire);
}

void ATurret::Fire()
{
	if (!bIsActive || !BulletClass) return;

	// Spawn bala
	FVector SpawnLoc = Muzzle->GetComponentLocation();
	FRotator SpawnRot = Muzzle->GetComponentRotation();

	GetWorld()->SpawnActor<ATurretBullet>(BulletClass, SpawnLoc, SpawnRot);

	// Efecto de Muzzle
	if (MuzzleEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleEffect, SpawnLoc, SpawnRot);
	}

	if (!bIsInfinite)
	{
		BulletCount--;
		if (BulletCount <= 0)
		{
			StopFire();

			// Reinicia ráfaga tras delay
			FTimerHandle TempHandle;
			GetWorldTimerManager().SetTimer(
				TempHandle,
				[this]() {
					BulletCount = 5; // ? Ojo: podrías usar una variable BaseBulletCount si quieres editable
					StartFire();
				},
				TimeFireDelay,
				false
			);
		}
	}
}