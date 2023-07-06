// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "AITypes.h"
//#include "AIController.h"
#include "Enemy.generated.h"
//#include "Components/FloatingPawnMovementComponent.h"
//class USphereComponent;
class UFloatingPawnMovement;
class APacman_Pawn;
class AAIController;
struct FAIRequestID;
struct FPathFollowingResult;
class USoundCue;
UENUM(BlueprintType) enum class EEnemyState :uint8 {
	Default,
	Idle
};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEnemyStateChangeEvent, EEnemyState, NewState);
UCLASS()
class PACMAN_API AEnemy : public APawn
{
	GENERATED_BODY()

public:
	 virtual void BeginPlay() override;
	 AAIController* EnemyAIController;
	AEnemy();
	UPROPERTY(EditDefaultsOnly) UFloatingPawnMovement* FloatMovement;
	//UPROPERTY(EditDefaultsOnly) USphereComponent* SphereCollision;
	UPROPERTY(EditDefaultsOnly) UStaticMeshComponent* Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)EEnemyState State = EEnemyState::Default;
	void OnMoveToCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result);
	UFUNCTION(BlueprintCallable) void Hunt();
	UFUNCTION(BlueprintCallable) void Idle();
	FEnemyStateChangeEvent& OnStateChanged() {	return StateChangedEvent;	}
private:
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FEnemyStateChangeEvent StateChangedEvent;
	bool GameFinished;
protected:
	UPROPERTY(EditDefaultsOnly) USoundCue* Sound;
	UPROPERTY()	APacman_Pawn* Pacman;
	void RestartWorld();
	bool WorldRestarting = false;
};
