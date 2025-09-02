// Fill out your copyright notice in the Description page of Project Settings. 

#include "HealthComponent.h" 

// Sets default values for this component's properties 
UHealthComponent::UHealthComponent() 
{ 
	PrimaryComponentTick.bCanEverTick = true; 
	MaxHealth = 100.0f; 
} 
// Called when the game starts 
void UHealthComponent::BeginPlay() 
{ 
	Super::BeginPlay(); 
	currentHealth = MaxHealth; 
	MyOwner = GetOwner(); 
	if (IsValid(MyOwner)) 
	{ 
		MyOwner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::TakingDamage); 
	} 
} 
void UHealthComponent::TakingDamage(AActor* DamageActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser) 
{ 
	if (Damage <= 0.0f || bIsDead) 
	{ 
		return; 
	} 

	currentHealth = FMath::Clamp(currentHealth - Damage, 0.0f, MaxHealth); 
	
	if (currentHealth == 0.0f) 
	{ 
		bIsDead = true; 
	} 
	
	OnHealthChangeDelegate.Broadcast(this, DamageActor, Damage, DamageType, InstigatedBy, DamageCauser); 
	if (bDebug) 
	{ 
		UE_LOG(LogTemp, Log, TEXT("Health: %s"), *FString::SanitizeFloat(currentHealth)); 
	} 
} 

// Called every frame 
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{ 
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction); 
}