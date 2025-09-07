#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class UDamageType;
class AFTPSCharacter;

UCLASS()
class CDT_FPS_CMASMAS_API AWeapon : public AActor
{
	GENERATED_BODY()

public:
	AWeapon();

protected:
	// --- Damage ---
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon|Combat")
	float Damage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Combat")
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Owner")
	AFTPSCharacter* CurrentOwnerCharacter;

	// --- Ammo ---
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Ammo")
	int32 CurrentMagAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Ammo")
	int32 MaxMagAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Ammo")
	int32 CurrentAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Ammo")
	int32 MaxAmmo;

	// --- Fire control ---
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Combat")
	float FireRate;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Combat")
	bool bIsShooting;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon|Combat")
	bool bCanReload;

	FTimerHandle TimerHandle_FireRate;

	// --- Reaload ---
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Reload")
	float ReloadTime = 2.0f;

	bool bIsReloading = false;

	FTimerHandle TimerHandle_Reload;

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Weapon")
	void BP_StartAction();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Weapon")
	void BP_StopAction();

	UFUNCTION(BlueprintCallable)
	void SetCharacterOwner(AFTPSCharacter* NewOwner);

	void CheckReloadState();
	virtual void ConsumeAmmo();

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	virtual void StartAction();

	UFUNCTION(BlueprintCallable)
	virtual void StopAction();

	UFUNCTION(BlueprintCallable)
	virtual void Reload();

	UFUNCTION(BlueprintCallable)
	virtual void FinishReload();

	UFUNCTION(BlueprintCallable, Category = "Weapon|Reload")
	float GetReloadTime() const { return ReloadTime; }

	UFUNCTION(BlueprintCallable, Category = "Weapon|Reload")
	float GetReloadProgress() const;

public:

	// Devuelve la munición actual del cargador
	UFUNCTION(BlueprintCallable, Category = "Weapon|Ammo")
	FORCEINLINE int32 GetCurrentMagAmmo() const { return CurrentMagAmmo; }

	// Devuelve la munición máxima del cargador
	UFUNCTION(BlueprintCallable, Category = "Weapon|Ammo")
	FORCEINLINE int32 GetMaxMagAmmo() const { return MaxMagAmmo; }

	// Devuelve la munición actual del jugador
	UFUNCTION(BlueprintCallable, Category = "Weapon|Ammo")
	FORCEINLINE int32 GetCurrentAmmo() const { return CurrentAmmo; }

	// Devuelve la munición máxima del jugador
	UFUNCTION(BlueprintCallable, Category = "Weapon|Ammo")
	FORCEINLINE int32 GetMaxAmmo() const { return MaxAmmo; }

	// Devuelve el tiempo entre cada disparo del arma
	UFUNCTION(BlueprintCallable, Category = "Weapon|Fire")
	FORCEINLINE float GetFireRate() const { return FireRate; }

	// Devuelve si el arma está disparando
	UFUNCTION(BlueprintCallable, Category = "Weapon|Fire")
	FORCEINLINE bool IsShooting() const { return bIsShooting; }

	// Devuelve si el arma puede recargar
	UFUNCTION(BlueprintCallable, Category = "Weapon|Reload")
	FORCEINLINE bool CanReload() const { return bCanReload; }

	// Devuelve si el arma está recargando
	UFUNCTION(BlueprintCallable, Category = "Weapon|Reload")
	FORCEINLINE bool IsReloading() const { return bIsReloading; }

	void AddAmmo(int32 Amount) { CurrentAmmo = FMath::Clamp(CurrentAmmo + Amount, 0, MaxAmmo); }
};