// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "FoodInterface.h"
#include "FoodActor.h"
#include "Pacman_Pawn.generated.h"
class UFloatingPawnMovement;
class USoundCue;
//class UStaticMesh;
UCLASS()
class PACMAN_API APacman_Pawn : public APawn, public IFoodInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APacman_Pawn();
	FVector Movement_Direction;
	void Change_Direction (FVector New_Direction);
	UPROPERTY(VisibleAnywhere)	UStaticMeshComponent* Mesh;
	
	UPROPERTY(VisibleAnywhere)	UFloatingPawnMovement* Floating_Movement_Component;
	UFUNCTION(BlueprintCallable) void ChangeSpeed( bool SpeedIncreasing);
	UPROPERTY(EditDefaultsOnly) USoundCue* TeleportationSound;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	bool Swallow = false;
	const FName TestTraceTag="TestTraceTag";


public:	
	bool Started;
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	 void FoodOverlaped_Implementation() ;
	// void TestCppFunction() override;
	bool GameFinished;
	UPROPERTY(EditDefaultsOnly)UStaticMesh* MeshClosedMouth;
	UPROPERTY(EditDefaultsOnly)UStaticMesh* MeshOpenMouth;
	UFUNCTION(BlueprintCallable)void ChangeMesh(UStaticMesh* NewMesh);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	

};
