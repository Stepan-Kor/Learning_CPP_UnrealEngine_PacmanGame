// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/MeshComponent.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "FoodActor.generated.h"
class USoundCue;
UENUM(BlueprintType) enum class EFoodType :uint8 {Regular,PowerUp};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FoodEatenEvent, EFoodType, Food_Type);
UCLASS()
class PACMAN_API AFoodActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFoodActor();
	//FoodActor.Mesh is not blueprint visible(BlueprintReadOnly or BlueprintReadWrite).Please fix mark up or cease accessing as this will be made an error in a future release.Get Mesh

	UPROPERTY(EditDefaultsOnly)USceneComponent* SceneComponent;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)UStaticMeshComponent* Mesh;
	//UFUNCTION(VisibleAnywhere)
	UFUNCTION(BlueprintCallable) void Consume();
	UPROPERTY(EditAnywhere)EFoodType Food_Type = EFoodType::Regular;
	UPROPERTY(BlueprintAssignable, BlueprintCallable) FoodEatenEvent FoodConsumedEvent;
	
protected:
	
	UFUNCTION()		void OnBeginOverlap(class UPrimitiveComponent* HitComp,
			class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bfromSweep, const FHitResult& SweepResult);
	UPROPERTY() UStaticMesh* StaticMeshFruit;
	UMaterial* FruitMaterial;
		UStaticMesh* TestStaticMesh;


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//UPROPERTY(EditAnywhere)
		USoundCue* ConsumptionSound;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable) bool TestFunction(FString Message);

};
