#include "Weapon.h"
#include "FTPSCharacter.h"
#include "TimerManager.h"

AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	Damage = 100.0f;

	MaxMagAmmo   = 30;
	CurrentMagAmmo = MaxMagAmmo;

	MaxAmmo      = 90;
	CurrentAmmo  = MaxAmmo;

	FireRate     = 0.15f;
	bIsShooting  = false;
	bCanReload   = false;
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	CheckReloadState();
}

void AWeapon::SetCharacterOwner(AFTPSCharacter* NewOwner)
{
	if (IsValid(NewOwner))
	{
		SetOwner(NewOwner);
		CurrentOwnerCharacter = NewOwner;
	}
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeapon::StartAction()
{
	if (bIsShooting) return;
	if (CurrentMagAmmo <= 0) return;

	bIsShooting = true;

	BP_StartAction();
	ConsumeAmmo();

	// Cooldown del disparo
	GetWorldTimerManager().SetTimer(TimerHandle_FireRate, this, &AWeapon::StopAction, FireRate, false);
}

void AWeapon::StopAction()
{
	bIsShooting = false;
	BP_StopAction();
}

void AWeapon::ConsumeAmmo()
{
	if (CurrentMagAmmo > 0)
	{
		CurrentMagAmmo--;
	}
	CheckReloadState();
}

void AWeapon::Reload()
{
	if (bIsReloading) return;

	if (CanReload())
	{
		bIsReloading = true;

		// Detener disparo
		AFTPSCharacter* OwnerCharacter = Cast<AFTPSCharacter>(GetOwner());
		if (OwnerCharacter)
		{
			OwnerCharacter->StopWeaponAction();
		}

		// Iniciar timer para el tiempo de recarga
		GetWorldTimerManager().SetTimer(
			TimerHandle_Reload,
			this,
			&AWeapon::FinishReload,
			ReloadTime,
			false
		);
	}
}

void AWeapon::FinishReload()
{
	bIsReloading = false;

	int32 AmmoNeeded = MaxMagAmmo - CurrentMagAmmo;
	int32 AmmoToReload = FMath::Min(AmmoNeeded, CurrentAmmo);

	CurrentMagAmmo += AmmoToReload;
	CurrentAmmo -= AmmoToReload;

	// Actualizar estado de recarga después de terminar
	CheckReloadState();
}


void AWeapon::CheckReloadState()
{
	bCanReload = (CurrentMagAmmo < MaxMagAmmo && CurrentAmmo > 0);
}

// Progreso de tiempo de recarga para el WidgetBlueprint
float AWeapon::GetReloadProgress() const
{
	if (!bIsReloading) return 0.0f;
	
	if (!GetWorldTimerManager().TimerExists(TimerHandle_Reload))
		return 0.0f;

	// Se obtiene el tiempo faltante para completar la recarga
	float TimeRemaining = GetWorldTimerManager().GetTimerRemaining(TimerHandle_Reload);
	return 1.0f - (TimeRemaining / ReloadTime);
}
