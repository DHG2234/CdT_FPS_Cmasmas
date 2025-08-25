// Fill out your copyright notice in the Description page of Project Settings.


#include "FTPSCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Weapon.h"

// Sets default values
AFTPSCharacter::AFTPSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FPSCameraSocketName = "FPS_Camera";

	FPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FPS_CameraComponent"));
	FPSCameraComponent->bUsePawnControlRotation = true;
	FPSCameraComponent->SetupAttachment(GetMesh(), FPSCameraSocketName);

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("TPS_SpringArmComponent"));
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->SetupAttachment(RootComponent);

	TPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TPS_CameraComponent"));
	TPSCameraComponent->SetupAttachment(SpringArmComponent);
}

// Called when the game starts or when spawned
void AFTPSCharacter::BeginPlay()
{
	Super::BeginPlay();

	FPSCameraComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("FPS_Camera"));
	
}

// Called every frame
void AFTPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CreateInitialWeapon();
}

// Called to bind functionality to input
void AFTPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Move_Forward", this, &AFTPSCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move_Right", this, &AFTPSCharacter::MoveRight);
	
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AFTPSCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AFTPSCharacter::StopJumping);

	PlayerInputComponent->BindAxis("Look_Up", this, &AFTPSCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Look_Right", this, &AFTPSCharacter::AddControllerYawInput);

	PlayerInputComponent->BindAction("Weapon", IE_Pressed, this, &AFTPSCharacter::StartWeaponAction);
	PlayerInputComponent->BindAction("Weapon", IE_Released, this, &AFTPSCharacter::StopWeaponAction);
}

void AFTPSCharacter::MoveForward(float value)
{
	AddMovementInput(GetActorForwardVector() * value);
}

void AFTPSCharacter::MoveRight(float value)
{
	AddMovementInput(GetActorRightVector() * value);
}
 
void AFTPSCharacter::Jump()
{
	Super::Jump();
}

void AFTPSCharacter::StopJumping()
{
	Super::StopJumping();
}

void AFTPSCharacter::AddControllerPitchInput(float val)
{
	if (bIsLookInversion == true) 
	{
		Super::AddControllerPitchInput(-val);
	}
	else {
		Super::AddControllerPitchInput(val);
	}
}

void AFTPSCharacter::AddControllerYawInput(float val)
{
	Super::AddControllerYawInput(val);
}

void AFTPSCharacter::AddKey(FName NewKey)
{
	DoorKeys.Add(NewKey);
}

bool AFTPSCharacter::HasKey(FName KeyTag)
{
	return DoorKeys.Contains(KeyTag);
}

void AFTPSCharacter::CreateInitialWeapon()
{
	if (IsValid(InitialWeaponClass))
	{
		CurrentWeapon = GetWorld()->SpawnActor<AWeapon>(InitialWeaponClass, GetActorLocation(), GetActorRotation());
		if (IsValid(CurrentWeapon))
		{
			CurrentWeapon->SetOwner(this);
			CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		}
	}
}

void AFTPSCharacter::StartWeaponAction()
{
	if (IsValid(CurrentWeapon))
	{
		CurrentWeapon->StartAction();
	}
}

void AFTPSCharacter::StopWeaponAction()
{
	if (IsValid(CurrentWeapon))
	{
		CurrentWeapon->StopAction();
	}
}

FVector AFTPSCharacter::GetPawnViewLocation() const
{
	if (IsValid(FPSCameraComponent) && bUseFirstPersonView)
	{
		return FPSCameraComponent->GetComponentLocation();
	}
	if (IsValid(TPSCameraComponent) && !bUseFirstPersonView)
	{
		return TPSCameraComponent->GetComponentLocation();
	}
	return Super::GetPawnViewLocation();
}