// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"
#include "Components/BoxComponent.h"
#include "FTPSCharacter.h"


ADoor::ADoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CustomRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("CustomRoot"));
	RootComponent = CustomRootComponent;

	//Create our Default Components
	DoorFrame = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorFrame"));
	Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	DoorTimelineComp = CreateDefaultSubobject<UTimelineComponent>(TEXT("DoorTimelineComp"));
	DoorProxVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("DoorProximityVolume"));

	//Setup our Attachments
	DoorFrame->SetupAttachment(CustomRootComponent);
	Door->AttachToComponent(DoorFrame, FAttachmentTransformRules::KeepRelativeTransform);
	DoorProxVolume->AttachToComponent(DoorFrame, FAttachmentTransformRules::KeepRelativeTransform);

	DoorProxVolume->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	DoorProxVolume->SetCollisionResponseToAllChannels(ECR_Ignore);
	DoorProxVolume->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	DoorTag = "KeyA";
}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();

	//Binding our float track to our UpdateTimelineComp Function's output
	UpdateFunctionFloat.BindDynamic(this, &ADoor::UpdateTimelineComp);

	//If we have a float curve, bind it's graph to our update function
	if (DoorTimelineFloatCurve)
	{
		DoorTimelineComp->AddInterpFloat(DoorTimelineFloatCurve, UpdateFunctionFloat);
	}
	//Binding our Proximity Box Component to our Overlap Functions
	//DoorProxVolume->OnComponentEndOverlap.AddDynamic(this, &ADoor::CloseDoor);
}

// Called every frame
void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DoorProxVolume->OnComponentBeginOverlap.AddDynamic(this, &ADoor::CheckKeyFromPlayer);
}

void ADoor::CheckKeyFromPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool BFromSweep, const FHitResult& SweepResult)
{
	if (bIsOpen) 
	{
		return;
	}
	if (IsValid(OtherActor))
	{
		AFTPSCharacter* OverlapCharacter = Cast<AFTPSCharacter>(OtherActor);
		if (IsValid(OverlapCharacter))
		{
			if (OverlapCharacter->HasKey(DoorTag) && OverlapCharacter->GetCharacterType() == FTPSCharacterType::CharacterType_Player)
			{
				DoorOpen();
			}
		}
	}
}

void ADoor::UpdateTimelineComp(float Output)
{
	// Create and set our Door's new relative location based on the output from our Timeline Curve
	FRotator DoorNewRotation = FRotator(0.0f, Output, 0.f);
	Door->SetRelativeRotation(DoorNewRotation);
}

void ADoor::DoorOpen()
{
	DoorTimelineComp->Play();

	bIsOpen = true;
}

/*
void ADoor::CloseDoor(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	DoorTimelineComp->Reverse();
}
*/