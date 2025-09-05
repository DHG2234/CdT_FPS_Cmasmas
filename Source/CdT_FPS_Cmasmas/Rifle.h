#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Rifle.generated.h"

UCLASS()
class CDT_FPS_CMASMAS_API ARifle : public AWeapon
{
	GENERATED_BODY()

protected:

	// Part�culas
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FX")
	UParticleSystem* MuzzleEffect;

	// Part�cula que se reproduce al impactar (hit)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FX")
	UParticleSystem* ImpactEffect;

	// Part�cula al impactar un enemigo
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FX")
	UParticleSystem* EnemyImpactEffect;

	// Nombre del socket
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName MuzzleSocketName = "MuzzleFlashSocket";

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LineTrace")
	bool bDrawLineTrace;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LineTrace")
	float TraceLine;

public:
	ARifle();

protected:
	virtual void StartAction() override;
	virtual void StopAction() override;
};