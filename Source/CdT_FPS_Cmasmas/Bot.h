// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Bot.generated.h"

class UStaticMeshComponent;
class AFTPSCharacter;
class UHealthComponent;
class USphereComponent;
class UParticleSystemComponent;
class UAudioComponent;

UCLASS()
class CDT_FPS_CMASMAS_API ABot : public APawn
{
	GENERATED_BODY()
	
public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UStaticMeshComponent* BotMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UHealthComponent* HealthComponent;

	UMaterialInstanceDynamic* BotMaterial;

	UPROPERTY(BlueprintReadOnly, Category = "Bot Movement")
	bool bIsExpired;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bot Effect")
	float ExplosionDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bot Effect")
	float ExplosionRadius;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bot Effect")
	float ExplosionDetector;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	USphereComponent* SelfDestructionDetectorComponent;

	// Particulas
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bot FX")
	UParticleSystemComponent* ExplosionFXComponent;

	// Sonido
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bot FX")
	UAudioComponent* ExplosionAudioComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Bot Effect")
	bool bIsStartingCoundown;

	FTimerHandle TimerHandleDamage;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug")
	bool bDebug;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bot Movement")
	float MinDistanceToTarget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bot MOvement")
	float ForceMagnitud;

	UPROPERTY(BlueprintReadOnly, Category = "Bot")
	FVector NextPathPoint;

	UPROPERTY(BlueprintReadOnly, Category = "Reference")
	AFTPSCharacter* PlayerCharacter;

	UFUNCTION()
	void TakingDamage(UHealthComponent* CurrentHealthComponent, AActor* DamagedActor, 
		float Damage, const UDamageType* DamageType, AController* InstigatedBy, 
		AActor* DamageCauser);

public:
	// Sets default values for this pawn's properties
	ABot();

	UFUNCTION(BlueprintCallable, Category = "Navigation")
	FVector GetNextPathPoint();

	void SelfDamage();
	void SelfDestruction();

	UFUNCTION()
	void StartCoundown(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
