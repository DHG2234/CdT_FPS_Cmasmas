// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Trap.generated.h"

class UBoxComponent;
class UHealthComponent;

UCLASS()
class CDT_FPS_CMASMAS_API ATrap : public AActor
{
	GENERATED_BODY()
	
public:
	ATrap();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* CollisionBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trap")
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trap")
	float ChangeTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trap")
	float Damage;

	FTimerHandle TimerHandle;

protected:

	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void ChangeDirection();

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);
};