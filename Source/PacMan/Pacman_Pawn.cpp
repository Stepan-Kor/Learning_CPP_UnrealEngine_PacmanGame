// Fill out your copyright notice in the Description page of Project Settings.


#include "Pacman_Pawn.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include <GameFramework/FloatingPawnMovement.h>
#include "Kismet/KismetSystemLibrary.h"
#include "Sound/SoundCue.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
APacman_Pawn::APacman_Pawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh of Pacman"));
	Floating_Movement_Component = CreateDefaultSubobject< UFloatingPawnMovement>(TEXT("Float Movement"));
	Mesh->SetEnableGravity(false);
	
	Mesh->SetCanEverAffectNavigation(false);
	
	ConstructorHelpers::FObjectFinder<USoundCue>SoundAsset(TEXT("SoundCue'/Game/Pacman/Audio/AS_TeleportCue.AS_TeleportCue'"));
	if(SoundAsset.Succeeded())TeleportationSound = SoundAsset.Object;
	ConstructorHelpers::FObjectFinder<UStaticMesh>ClosedMesh(TEXT("StaticMesh'/Game/Pacman/Meshes/SM_PacmanClosed.SM_PacmanClosed'"));
	if (ClosedMesh.Succeeded())MeshClosedMouth = ClosedMesh.Object;
	ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("StaticMesh'/Game/Pacman/Meshes/SM_PacmanOpen.SM_PacmanOpen'"));
	if (MeshAsset.Succeeded()) { 
		UStaticMesh* Asset = MeshAsset.Object;
		MeshOpenMouth = Asset;
		Mesh->SetStaticMesh(Asset);
	}

}


void APacman_Pawn::Change_Direction(FVector New_Direction)
{
	if (Started) {
		if (abs(New_Direction.X) > abs(New_Direction.Y)) {
			if (New_Direction.X > 0)APacman_Pawn::SetActorRotation(FRotator(0, 0, 0));
			else APacman_Pawn::SetActorRotation(FRotator(180, 00, 360));
		}
		else {
			if (New_Direction.Y > 0)APacman_Pawn::SetActorRotation(FRotator(0, 90, 0));
			else APacman_Pawn::SetActorRotation(FRotator(0, -90, 0));
		}
	}
}


// Called when the game starts or when spawned
void APacman_Pawn::BeginPlay()
{
	Super::BeginPlay();
	
	
}

void APacman_Pawn::ChangeSpeed(bool SpeedIncreasing)
{
	if (SpeedIncreasing) { 
		Floating_Movement_Component->MaxSpeed = 1500;
		Floating_Movement_Component->Acceleration = 10000;
		
		FLatentActionInfo FAI;//= FLatentActionInfo(0, 1, "ChangeSpeed", this);
		FAI.Linkage = 0;
		FAI.UUID = 0;
		FAI.CallbackTarget = this;
		FAI.ExecutionFunction = "ChangeSpeed";
		UKismetSystemLibrary::RetriggerableDelay(GetWorld(), 3, FAI);
		//GetWorldTimerManager().SetTimerForNextTick([asdf, this]() {ChangeSpeed(false); });

		//GetWorldTimerManager().SetTimer(UnUsedHandle, this, &AEnemy::RestartWorld, 5.5, false);
		//GetWorldTimerManager().SetTimer(UnUsedHandle, this, &APacman_Pawn::ChangeSpeed(SpeedIncreasing), 3, false);
		/*
		FTimerHandle UnUsedHandle;
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "ChangeSpeed", false);
		GetWorldTimerManager().SetTimer(UnUsedHandle, Delegate, 3, false);
		*/
		//UKismetSystemLibrary::RetriggerableDelay(GetWorld(),3,)
		}
	else {
		Floating_Movement_Component->MaxSpeed = 750;
		Floating_Movement_Component->Acceleration = 4000;
		//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Turquoise, "Pacman normalizes speed");
	}
}

// Called every frame
void APacman_Pawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!GameFinished) {
		if (Started)AddMovementInput(GetActorForwardVector());
		if (abs(this->GetActorLocation().X) > 1800) {
			if (this->GetActorLocation().X > 0)this->SetActorLocation(FVector(-1799, this->GetActorLocation().Y, this->GetActorLocation().Z));
			else this->SetActorLocation(FVector(1799, this->GetActorLocation().Y, this->GetActorLocation().Z));
			if (IsValid(TeleportationSound))UGameplayStatics::SpawnSound2D(GetWorld(), TeleportationSound, 0.5);
			//UGameplayStatics::PlaySound2D(this, ConsumptionSound, 2);
		}
	}
}

// Called to bind functionality to input
void APacman_Pawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APacman_Pawn::ChangeMesh(UStaticMesh* NewMesh) {	if(IsValid(NewMesh))Mesh->SetStaticMesh(NewMesh);	}


//void APacman_Pawn::TestCppFunction() { GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Blue, "overrided cpp func called"); }
									
void APacman_Pawn::FoodOverlaped_Implementation()
{
	//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Orange, "Pacman overlaped by food");
	Mesh->SetStaticMesh(MeshClosedMouth);
	FTimerHandle UnUsedHandle;
	FTimerDelegate MouthDelegate = FTimerDelegate::CreateUObject(this, &APacman_Pawn::ChangeMesh, MeshOpenMouth);
	GetWorldTimerManager().SetTimer(UnUsedHandle, MouthDelegate, 0.3, false);
	
	
	FCollisionQueryParams TraceParameters = FCollisionQueryParams(FName(TEXT("wtf?")), false, this);
	//GetWorld()->DebugDrawTraceTag = TestTraceTag;
	//TraceParameters.TraceTag = TestTraceTag;
	TraceParameters.bTraceComplex = false;
	TraceParameters.bReturnPhysicalMaterial = false; 
	FHitResult HitResultat(ForceInit);
	FVector EndOfTrace = FVector(0, -150, 0) + this->GetActorLocation();
	GetWorld()->LineTraceSingleByChannel(HitResultat, this->GetActorLocation(), EndOfTrace, ECC_WorldStatic, TraceParameters);
	if (HitResultat.bBlockingHit) {
		if (IsValid(HitResultat.GetActor())) {
		//FString TempString = HitResultat.Actor->GetName();
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("My Location is: %s"),*GetActorLocation().ToString()));

		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::MakeRandomColor(),FString::Printf(TEXT("traced to actor %s"),*HitResultat.Actor->GetName()));
		//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Blue, "overrided cpp func called");
		}
	}

	//Mesh->SetVectorParameterValueOnMaterials("Base Color", UKismetMathLibrary::RandomUnitVector());
	//GetWorldTimerManager().SetTimer(UnUsedHandle, this, &AEnemy::RestartWorld, 5.5, false);

}
