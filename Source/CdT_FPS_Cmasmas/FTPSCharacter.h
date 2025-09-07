// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FTPSCharacter.generated.h"

UENUM()
enum class FTPSCharacterType : uint8
{
	CharacterType_Player UMETA(DisplayName = "Player"),
	CharacterType_Enemy UMETA(DisplayName = "Enemy"),
};

class UCameraComponent;
class USpringArmComponent;
class AWeapon;
class UAnimMontage;
class UAnimInstance;
class UHealthComponent;
class AFTPSGameMode;

UCLASS()
class CDT_FPS_CMASMAS_API AFTPSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFTPSCharacter();

	virtual FVector GetPawnViewLocation() const override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aiming")
	bool bIsLookInversion;

	//Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* FPSCameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aiming")
	FName FPSCameraSocketName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* TPSCameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aiming")
	bool bUseFirstPersonView;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Key")
	TArray<FName> DoorKeys;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FTPSCharacterType CharacterType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<AWeapon>InitialWeaponClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	AWeapon* CurrentWeapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* MeleeMontage;

	UAnimInstance* MyAnimInstance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCapsuleComponent* MeleeDetectorComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Melee")
	FName MeleeSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Melee")
	float MeleeDamage;

	UPROPERTY(BlueprintReadOnly, Category = "Melee")
	bool bIsDoingMelee;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Melee")
	bool bCanUseWeapon;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UHealthComponent* HealthComponent;

	AFTPSGameMode* GameModeReference;

	FTimerHandle TimerHandle_AutoFire;
	bool bIsFiringWeapon;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float value);
	void MoveRight(float value);
	virtual void Jump() override;
	virtual void StopJumping() override;

	void InitialReference();
	void StartMelee();
	void StopMelee();

	void HandleAutoFire();

	UFUNCTION()
	void MakeMeleeDamage(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void AddControllerPitchInput(float val) override;
	virtual void AddControllerYawInput(float val) override;

	void AddKey(FName NewKey);
	bool HasKey(FName KeyTag);

	UFUNCTION(BlueprintCallable)
	FTPSCharacterType GetCharacterType() { return CharacterType; };

	void CreateInitialWeapon();
	void StartWeaponAction();
	void StopWeaponAction();

	void SetMeleeDetectorCollision(ECollisionEnabled::Type NewCollisionState);

	void SetActionsState(bool NewState);
	
	UFUNCTION()
	void OnHealthChange(UHealthComponent* CurrentHealthComponent, AActor* DamagedActor, float Damage,
		const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

	void ReloadWeapon();

	UFUNCTION(BlueprintCallable, Category = "Melee")
	bool IsDoingMelee() const { return bIsDoingMelee; }

public:

	// Respawn system
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Respawn")
	FVector InitialPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Respawn")
	FRotator InitialRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Respawn")
	int32 InitialHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Respawn")
	int32 MaxRespawns;

	UPROPERTY(BlueprintReadOnly, Category = "Respawn")
	int32 CurrentRespawns;

	void Respawn();

public:

	int32 GetRespawns() const { return CurrentRespawns; }
	int32 GetMaxRespawns() const { return MaxRespawns; }
	void AddRespawn(int32 Amount) { CurrentRespawns = FMath::Clamp(CurrentRespawns + Amount, 0, MaxRespawns); }

	class UHealthComponent* GetHealthComponent() const { return HealthComponent; }
	class AWeapon* GetCurrentWeapon() const { return CurrentWeapon; }
};