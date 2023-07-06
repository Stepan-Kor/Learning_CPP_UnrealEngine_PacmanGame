// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveFile.generated.h"

/**
 * 
 */
UCLASS()
class PACMAN_API USaveFile : public USaveGame
{
	GENERATED_BODY()
public:
		UPROPERTY(EditAnywhere) int MaxPoints=0;
	
};
