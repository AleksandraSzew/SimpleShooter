// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "Gun.h"
#include "Grenade.h"
#include "Components/CapsuleComponent.h"
#include "SimpleShooterGameModeBase.h"
#include <Kismet/GameplayStatics.h>
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
	Health = MaxHealth;
	Gun = GetWorld()->SpawnActor<AGun>(GunClass);
	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
	Gun->SetOwner(this);
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	


}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AShooterCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Shoot"), EInputEvent::IE_Pressed, this, &AShooterCharacter::Shoot);
	PlayerInputComponent->BindAction(TEXT("SwapWeapon"), EInputEvent::IE_Pressed, this, &AShooterCharacter::SwapWeapon);
	PlayerInputComponent->BindAction(TEXT("Grenade"), EInputEvent::IE_Pressed, this, &AShooterCharacter::ThrowGrenade);

	//controller
	PlayerInputComponent->BindAxis(TEXT("LookUpRete"), this, &AShooterCharacter::LookUpRate);
	PlayerInputComponent->BindAxis(TEXT("LookRightRete"), this, &AShooterCharacter::LookRightRate);
}
void AShooterCharacter::MoveForward(float axisValue)
{
	AddMovementInput(GetActorForwardVector() * axisValue);
}
void AShooterCharacter::MoveRight(float axisValue)
{
	AddMovementInput(GetActorRightVector() * axisValue);
}
void AShooterCharacter::LookUpRate(float axisValue)
{
	AddControllerPitchInput(axisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}
void AShooterCharacter::LookRightRate(float axisValue)
{
	AddControllerYawInput(axisValue * RotationRate * GetWorld()->GetDeltaSeconds());

}
void AShooterCharacter::Shoot()
{
	Gun->PullTrigger();
}
float AShooterCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float DamageApplied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	float DamageToApply = FMath::Min(Health, DamageApplied);
	Health -= DamageToApply;

	UE_LOG(LogTemp, Warning, TEXT("Health left %f"), Health);


	if (IsCharacterDead())
	{
		
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		ASimpleShooterGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ASimpleShooterGameModeBase>();
		if (GameMode != nullptr)
		{
			GameMode->PawnKilled(this);
		}
		DetachFromControllerPendingDestroy();
	}
	return DamageToApply;	
}

bool AShooterCharacter::IsCharacterDead() const
{
	return Health <=0;
}

float AShooterCharacter::getHealthPercantage() const
{

	return Health / MaxHealth;
}

void AShooterCharacter::SwapWeapon()
{
	UE_LOG(LogTemp, Warning, TEXT("Swapping Weapon"));
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), SwapWeaponSound, this->GetTargetLocation());
	
}

void AShooterCharacter::ThrowGrenade()
{	
	if (GrenadeClass)
	{
		Granade = GetWorld()->SpawnActor<AGrenade>(GrenadeClass);
		if (Granade)
		{
			
			if (ThrowAnimation)
				{	
					GetMesh()->PlayAnimation(ThrowAnimation, false);
				}
				
			
			Granade->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale,FName("canon_barrel_C"));
		
		}
		OnGrenadeRelease();
	}
	
	
}
void AShooterCharacter::OnGrenadeRelease()
{
	if (Granade)
	{
		Granade->OnReleased(UKismetMathLibrary::GetForwardVector(GetControlRotation()));			
	}
}