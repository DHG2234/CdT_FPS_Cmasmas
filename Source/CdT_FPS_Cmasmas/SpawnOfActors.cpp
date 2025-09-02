#include "SpawnOfActors.h"
#include "Engine/World.h"
#include "TimerManager.h"

ASpawnOfActors::ASpawnOfActors()
{
	PrimaryActorTick.bCanEverTick = true;

	bSpawnIsActive = false;
	CurrentActorsSpawned = 0;
	CurrentActorsInWorld = 0;

	MaxActorToSpawn = 10;
	MaxActorSpawned = 5;
	SpawnTime = 2.0f;
}

void ASpawnOfActors::BeginPlay()
{
	Super::BeginPlay();

	StartSpawn();
}

void ASpawnOfActors::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASpawnOfActors::SpawnBot()
{
	if (!bSpawnIsActive || !ActorToSpawn) return;

	if (CurrentActorsSpawned >= MaxActorToSpawn)
	{
		GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
		return;
	}

	if (CurrentActorsInWorld >= MaxActorSpawned) return;

	UWorld* World = GetWorld();
	if (World)
	{
		FVector Location = GetActorLocation();
		FRotator Rotation = GetActorRotation();

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;

		AActor* SpawnedActor = World->SpawnActor<AActor>(ActorToSpawn, Location, Rotation, SpawnParams);

		if (SpawnedActor)
		{
			CurrentActorsSpawned++;
			CurrentActorsInWorld++;

			SpawnedActor->OnDestroyed.AddDynamic(this, &ASpawnOfActors::OnActorDestroyed);
		}
	}
}

void ASpawnOfActors::OnActorDestroyed(AActor* DestroyedActor)
{
	CurrentActorsInWorld = FMath::Max(0, CurrentActorsInWorld - 1);
}

void ASpawnOfActors::SetSpawnIsActive(bool bNewActive)
{
	bSpawnIsActive = bNewActive;

	if (bSpawnIsActive && CurrentActorsSpawned < MaxActorToSpawn)
	{
		if (!GetWorldTimerManager().IsTimerActive(SpawnTimerHandle))
		{
			StartSpawn();
		}
	}
}

void ASpawnOfActors::StartSpawn()
{
	if (SpawnTime > 0.f)
	{
		GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &ASpawnOfActors::SpawnBot,
			SpawnTime, true, 0.0f);
	}
}

void ASpawnOfActors::StopSpawn()
{
	if (GetWorldTimerManager().IsTimerActive(SpawnTimerHandle))
	{
		GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
	}
}