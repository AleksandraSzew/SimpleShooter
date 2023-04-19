// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

class AGun;

UCLASS()
class SIMPLESHOOTER_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();
	void Shoot();
	void SwapWeapon();
	void ThrowGrenade();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintPure)
		bool IsCharacterDead() const;
	UFUNCTION(BlueprintPure)
		float getHealthPercantage() const;
private:
	void MoveForward(float axisValue);
	void MoveRight(float axisValue);
	void LookUpRate(float axisValue);
	void LookRightRate(float axisValue);

UPROPERTY(EditAnywhere)
		float RotationRate = 7;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AGun> GunClass;

	UPROPERTY()
		AGun* Gun;

	UPROPERTY(EditDefaultsOnly)
		float MaxHealth = 100;
	UPROPERTY(VisibleAnywhere)
		float Health;
	UPROPERTY(EditAnywhere)
		USoundBase* SwapWeaponSound;
};
