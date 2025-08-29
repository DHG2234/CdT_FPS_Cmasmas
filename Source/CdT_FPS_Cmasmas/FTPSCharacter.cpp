// Fill out your copyright notice in the Description page of Project Settings.


#include "FTPSCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Weapon.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Components/CapsuleComponent.h"
#include "CdT_FPS_Cmasmas.h"
#include "Kismet/GameplayStatics.h"
#include "HealthComponent.h"
#include "FTPSGameMode.h"

// Sets default values
AFTPSCharacter::AFTPSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FPSCameraSocketName = "FPS_Camera";

	MeleeSocketName = "SC_Melee";
	MeleeDamage = 10.0f;

	FPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FPS_CameraComponent"));
	FPSCameraComponent->bUsePawnControlRotation = true;
	FPSCameraComponent->SetupAttachment(GetMesh(), FPSCameraSocketName);

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("TPS_SpringArmComponent"));
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->SetupAttachment(RootComponent);

	TPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TPS_CameraComponent"));
	TPSCameraComponent->SetupAttachment(SpringArmComponent);

	MeleeDetectorComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("MeleeDetectorComponent"));
	MeleeDetectorComponent->SetupAttachment(GetMesh(), MeleeSocketName);
	MeleeDetectorComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	MeleeDetectorComponent->SetCollisionResponseToChannel(COLLISION_ENEMY, ECR_Overlap);
	MeleeDetectorComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
}

// Called when the game starts or when spawned
void AFTPSCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitialReference();

	CreateInitialWeapon();

	MeleeDetectorComponent->OnComponentBeginOverlap.AddDynamic(this, &AFTPSCharacter::MakeMeleeDamage);
	
	HealthComponent->OnHealthChangeDelegate.AddDynamic(this, &AFTPSCharacter::OnHealthChange);
}

// Called every frame
void AFTPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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

	PlayerInputComponent->BindAction("Melee", IE_Pressed, this, &AFTPSCharacter::StartMelee);
	PlayerInputComponent->BindAction("Melee", IE_Released, this, &AFTPSCharacter::StopMelee);
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

void AFTPSCharacter::InitialReference()
{
	if (IsValid(GetMesh()))
	{
		MyAnimInstance = GetMesh()->GetAnimInstance();
	}

	GameModeReference = Cast<AFTPSGameMode>(GetWorld()->GetAuthGameMode());
}

void AFTPSCharacter::StartMelee()
{
	if (bIsDoingMelee)
	{
		return;
	}
	if (IsValid(MyAnimInstance) && IsValid(MeleeMontage))
	{
		MyAnimInstance->Montage_Play(MeleeMontage);
	}
	SetActionsState(true);
}

void AFTPSCharacter::StopMelee()
{
}

void AFTPSCharacter::SetActionsState(bool NewState)
{
	bIsDoingMelee = NewState;
	bCanUseWeapon = !NewState;
}

void AFTPSCharacter::OnHealthChange(UHealthComponent* CurrentHealthComponent, AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (HealthComponent->IsDead() && GetCharacterType() == FTPSCharacterType::CharacterType_Player)
	{
		if (IsValid(GameModeReference))
		{
			GameModeReference->GameOver(this);
		}
	}
}

void AFTPSCharacter::MakeMeleeDamage(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsValid(OtherActor))
	{
		UGameplayStatics::ApplyPointDamage(OtherActor, MeleeDamage, SweepResult.Location, 
			SweepResult, GetInstigatorController(), this, nullptr);
	}
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
	if (!bCanUseWeapon)
	{
		return;
	}
	if (IsValid(CurrentWeapon))
	{
		CurrentWeapon->StartAction();
	}
}

void AFTPSCharacter::StopWeaponAction()
{
	if (!bCanUseWeapon)
	{
		return;
	}
	if (IsValid(CurrentWeapon))
	{
		CurrentWeapon->StopAction();
	}
}

void AFTPSCharacter::SetMeleeDetectorCollision(ECollisionEnabled::Type NewCollisionState)
{
	MeleeDetectorComponent->SetCollisionEnabled(NewCollisionState);
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