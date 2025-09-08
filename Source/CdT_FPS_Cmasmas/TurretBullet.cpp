// Fill out your copyright notice in the Description page of Project Settings.

#include "TurretBullet.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "FTPSCharacter.h"

ATurretBullet::ATurretBullet()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	RootComponent = SphereCollision;
	SphereCollision->InitSphereRadius(10.f);
	SphereCollision->SetCollisionProfileName("BlockAllDynamic");
	SphereCollision->OnComponentHit.AddDynamic(this, &ATurretBullet::OnHit);

	BulletTrail = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("BulletTrail"));
	BulletTrail->SetupAttachment(RootComponent);

	SpeedBullet = 1500.f;
	Damage = 20.f;
}

void ATurretBullet::BeginPlay()
{
	Super::BeginPlay();

	// Se destruye tras 5 segundos si no impacta
	SetLifeSpan(5.0f);
}

void ATurretBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Move = GetActorForwardVector() * SpeedBullet * DeltaTime;
	AddActorWorldOffset(Move, true);
}

void ATurretBullet::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!OtherActor || OtherActor == this) return;

	// Si golpea al jugador
	AFTPSCharacter* Player = Cast<AFTPSCharacter>(OtherActor);
	if (Player)
	{
		if (HitPlayerParticle)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitPlayerParticle, Hit.Location, Hit.Normal.Rotation());
		}

		UGameplayStatics::ApplyDamage(Player, Damage, GetInstigatorController(), this, nullptr);
	}
	else
	{
		if (HitWorldParticle)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitWorldParticle, Hit.Location, Hit.Normal.Rotation());
		}
	}

	Destroy();
}