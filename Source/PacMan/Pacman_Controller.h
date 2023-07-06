// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Pacman_Pawn.h"

#include "PacmanWidget.h"
#include "Pacman_Controller.generated.h"
class APacman_Pawn;
/**
 * 
 */
UCLASS()
class PACMAN_API APacman_Controller : public APlayerController
{
	GENERATED_BODY()
	//APacman_Controller();
	void SetupInputComponent() override;
	APacman_Pawn* Pac_Pawn;
public:
	UPROPERTY(EditDefaultsOnly) TSubclassOf<UPacmanWidget> PlayerHudClass = UPacmanWidget::StaticClass();
	UPROPERTY(VisibleAnywhere) UPacmanWidget* PlayerHudWidget;
	void MoveUp();
	void MoveDown();
	void MoveLeft();
	void MoveRight();
	void StartGame();
	void ExitGame();
	virtual void BeginPlay() override;
	
};
