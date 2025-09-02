// SpawnOfActors.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnOfActors.generated.h"

UCLASS()
class CDT_FPS_CMASMAS_API ASpawnOfActors : public AActor
{
	GENERATED_BODY()

public:
	ASpawnOfActors();

protected:
	virtual void BeginPlay() override;

	FTimerHandle SpawnTimerHandle;

	void SpawnBot();

	UFUNCTION()
	void OnActorDestroyed(AActor* DestroyedActor);

	void SetSpawnIsActive(bool bNewActive);

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Spawner")
	void StartSpawn();

	UFUNCTION(BlueprintCallable, Category = "Spawner")
	void StopSpawn();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner", meta = (ExposeOnSpawn = true))
	bool bSpawnIsActive;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawner")
	int32 CurrentActorsSpawned;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawner")
	int32 CurrentActorsInWorld;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
	int32 MaxActorToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
	int32 MaxActorSpawned;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
	float SpawnTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
	TSubclassOf<AActor> ActorToSpawn;
};