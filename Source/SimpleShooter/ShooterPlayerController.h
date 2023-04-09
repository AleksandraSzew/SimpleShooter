// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ShooterPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API AShooterPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
		void GameHasEnded(AActor* EndGameFocus, bool bIsWinner) override;
private:
	UPROPERTY(Editanywhere)
		float RestartDelay = 3.f;
	FTimerHandle RestartTimer;

	UPROPERTY(Editanywhere)
		TSubclassOf<class UUserWidget>	LoseScreenClass;
	UPROPERTY(Editanywhere)
		TSubclassOf<class UUserWidget>	WinScreenClass;
};
