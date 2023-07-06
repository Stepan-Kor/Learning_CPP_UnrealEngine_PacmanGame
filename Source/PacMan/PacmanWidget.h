// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FoodActor.h"
#include "PacmanWidget.generated.h"
class UTextBlock;
/**
 * 
 */
UCLASS()
class PACMAN_API UPacmanWidget : public UUserWidget
{
	GENERATED_BODY()
		virtual bool Initialize();
		UPROPERTY(meta = (BindWidget)) class UButton* StartGame;
		UPROPERTY(meta = (BindWidget))  UTextBlock* TextBlockPoints;
		UPROPERTY(meta = (BindWidget))  UTextBlock* TextBlockMaxPoints;
		UPROPERTY(EditDefaultsOnly) class USoundCue* SuccessSoundCue;
		UPROPERTY(VisibleAnywhere) class USaveFile* LoadedGame;
	UFUNCTION(BlueprintCallable)	void StartGameButtonClicked();
	UFUNCTION() void PacmanWidget();
	UFUNCTION() void RestartGame();
public:
	UFUNCTION() void SomeFoodConsumed(EFoodType TypeofFood);
	int FoodEaten;
protected:
	int MaxPoints=0;
	bool GameFinished;
};
