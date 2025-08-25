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

	UPROPERTY(BlueprintReadWrite, Category = "Weapon")
	AWeapon* CurrentWeapon;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float value);
	void MoveRight(float value);
	virtual void Jump() override;
	virtual void StopJumping() override;

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
};
