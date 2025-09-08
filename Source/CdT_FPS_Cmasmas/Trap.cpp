// Fill out your copyright notice in the Description page of Project Settings.

// Trap.cpp

#include "Trap.h"
#include "Components/BoxComponent.h"
#include "HealthComponent.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"

// Sets default values
ATrap::ATrap()
{
	PrimaryActorTick.bCanEverTick = true;

	// Root collision box
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	RootComponent = CollisionBox;
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	Speed = 100.f;
	ChangeTime = 2.f;
	Damage = 20.f;
}

// Called when the game starts or when spawned
void ATrap::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(TimerHandle, this, &ATrap::ChangeDirection, ChangeTime, true);

	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ATrap::OnOverlap);
}

// Called every frame
void ATrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Move(0, 0, Speed * DeltaTime);
	AddActorLocalOffset(Move, true);
}

void ATrap::ChangeDirection()
{
	Speed *= -1;
}

void ATrap::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == this) return;

	// Buscar el componente de vida
	if (UHealthComponent* HealthComp = OtherActor->FindComponentByClass<UHealthComponent>())
	{
		UGameplayStatics::ApplyDamage(OtherActor, Damage, GetInstigatorController(), this, nullptr);
	}
}