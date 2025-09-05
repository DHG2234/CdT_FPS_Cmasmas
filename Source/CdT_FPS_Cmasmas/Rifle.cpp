#include "Rifle.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "CdT_FPS_Cmasmas.h"
#include "FTPSCharacter.h"

ARifle::ARifle()
{
	TraceLine = 100000.0f;
}

void ARifle::StartAction()
{
	if (bIsShooting || CurrentMagAmmo <= 0) return;
	if (CurrentOwnerCharacter && CurrentOwnerCharacter->IsDoingMelee()) return;


	Super::StartAction();

	AActor* CurrentOwner = GetOwner();
	if (IsValid(CurrentOwner))
	{
		FVector EyeLocation;
		FRotator EyeRotation;

		CurrentOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

		FVector ShootDirection = EyeRotation.Vector();
		FVector TraceEnd = EyeLocation + (ShootDirection * TraceLine);
		
		AFTPSCharacter* OwnerCharacter = Cast<AFTPSCharacter>(GetOwner());
		if (MuzzleEffect && OwnerCharacter && OwnerCharacter->GetMesh())
		{
			UGameplayStatics::SpawnEmitterAttached(
				MuzzleEffect,
				OwnerCharacter->GetMesh(),     // Componente al que se attachará
				MuzzleSocketName,              // Nombre del socket
				FVector::ZeroVector,           // Offset local
				FRotator::ZeroRotator,         // Rotación local
				EAttachLocation::SnapToTarget, // Snap para que coincida con el socket
				true                           // AutoDestroy
			);
		}


		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);
		QueryParams.AddIgnoredActor(CurrentOwner);
		QueryParams.bTraceComplex = true;

		FHitResult HitResult;
		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, EyeLocation, TraceEnd, COLLISION_WEAPON, QueryParams);

		if (bHit)
		{
			AActor* HitActor = HitResult.GetActor();
			if (IsValid(HitActor))
			{
				// Aplicar daño
				UGameplayStatics::ApplyPointDamage(HitActor, Damage, ShootDirection, HitResult, OwnerCharacter->GetController(), this, DamageType);

				// Determinar la partícula a usar según el tipo de colisión
				UParticleSystem* SelectedImpactEffect = nullptr;

				UPrimitiveComponent* HitComp = HitResult.GetComponent();
				if (HitComp)
				{
					ECollisionChannel ObjectType = HitComp->GetCollisionObjectType();

					if (ObjectType == COLLISION_ENEMY) // Ajusta según tu canal de colisión Enemy
					{
						SelectedImpactEffect = EnemyImpactEffect;
					}
					else
					{
						SelectedImpactEffect = ImpactEffect;
					}
				}

				// Spawn de la partícula
				if (SelectedImpactEffect)
				{
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SelectedImpactEffect, HitResult.ImpactPoint, HitResult.ImpactNormal.Rotation());
				}
			}
		}

		if (bDrawLineTrace)
		{
			DrawDebugLine(GetWorld(), EyeLocation, TraceEnd, FColor::Green, false, 1.0f, 0.0f, 1.0f);
		}
	}
}

void ARifle::StopAction()
{
	Super::StopAction();
}
