// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Checkpoint.generated.h"

class UCapsuleComponent;
class AFTPSCharacter;

UCLASS()
class CDT_FPS_CMASMAS_API ACheckpoint : public AActor
{
	GENERATED_BODY()

public:
	ACheckpoint();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCapsuleComponent* CapsuleComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Checkpoint")
	FVector RespawnOffset; // opcional, por si quieres aparecer un poquito desplazado

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnCheckpointOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};