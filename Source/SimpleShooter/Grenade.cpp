// Fill out your copyright notice in the Description page of Project Settings.


#include "Grenade.h"
#include "ShooterCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
// Sets default values
AGrenade::AGrenade()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);

	GrenadeSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
}

// Called when the game starts or when spawned
void AGrenade::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGrenade::OnReleased(FVector ForwardVector, float Power)
{
	ForwardVector *= Power;
	Mesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Mesh->SetSimulatePhysics(true);
	Mesh->SetAllPhysicsLinearVelocity(FVector::ZeroVector);
	Mesh->AddImpulse(ForwardVector);

	FTimerHandle TimeExplode;
	GetWorldTimerManager().SetTimer(TimeExplode, this, &AGrenade::Explode, FuseLenght, false);
}

void AGrenade::Explode()
{

	GrenadeSphere->SetWorldLocation(Mesh->GetComponentLocation());
	TArray<AActor*> OverlappingActors;
	GrenadeSphere->GetOverlappingActors(OverlappingActors, TSubclassOf<AShooterCharacter>());

	for (AActor* Actor : OverlappingActors)
	{
		if (AShooterCharacter* Character = Cast<AShooterCharacter>(Actor))
		{
			UE_LOG(LogTemp, Warning, TEXT("EXPLODED ON ACTOR"));
		}
	}
}