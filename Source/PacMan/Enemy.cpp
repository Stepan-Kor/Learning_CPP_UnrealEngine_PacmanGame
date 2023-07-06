// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
//#include <GameFramework/FloatingPawnMovement.h>
#include "GameFramework/FloatingPawnMovement.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "AIController.h"
#include "Pacman_Pawn.h"
#include "GameFrameWork/Controller.h"
#include "Kismet/GameplayStatics.h"
#include "AITypes.h"
#include "Sound/Soundcue.h"
#include "Navigation/PathFollowingComponent.h"
AEnemy::AEnemy()
{
	//Super::AEnemy();
	FloatMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Float Pawn Movement"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	ConstructorHelpers::FObjectFinder<UStaticMesh>FoundStaticMesh(TEXT("StaticMesh'/Game/Pacman/Meshes/SM_Enemy_Default.SM_Enemy_Default'"));
	if (FoundStaticMesh.Succeeded()) Mesh->SetStaticMesh(FoundStaticMesh.Object);
	Mesh->SetCanEverAffectNavigation(false);
	//SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision"));
	//SphereCollision->SetupAttachment(Mesh);
	//SphereCollision->SetSphereRadius(150,false);
	FloatMovement->MaxSpeed = 500;
	//if (GetController())EnemyController = Cast<AAIController>(GetController());
	//AConrtoller* EnemyController = GetController();
	//EnemyAIController = Cast<AAIController>(GetController());
	//EnemyAIController = GetController<AAIController>();
	//OwnerComp.GetAIOwner();
	/*if (IsValid(GetController<AAIController>())) {
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Green, "AI Controller Valid");
	}*/
	static ConstructorHelpers::FObjectFinder<USoundCue>DeathSound(TEXT("SoundCue'/Game/Pacman/Audio/AS_DeathCue.AS_DeathCue'"));
	Sound = DeathSound.Object;
	EnemyAIController = Cast<AAIController>(GetController());
}

void AEnemy::BeginPlay()
{
	EnemyAIController = Cast<AAIController>(GetController());
	Super::BeginPlay();

	FTimerHandle UnusedHandle;
	EnemyAIController->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &AEnemy::OnMoveToCompleted);
	
	//
	//if (IsValid(EnemyAIController)) {
	//	//AAIController* AIContr = Cast<AAIController>(GetController());
	//	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Green, "Controller valid");
	//	GetWorldTimerManager().SetTimer(UnusedHandle, this, &AEnemy::Hunt, 2, false);
	//	//Cast<AAIController>(GetController())->GetDefaultConfigFilename();
	//}
	//else GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, "Controller not valid");
}

void AEnemy::OnMoveToCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) {	
	if(GameFinished)  	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Green, "Player won");
	else {
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, "Gtcha!");
		APacman_Pawn* PlayerChar = Cast<APacman_Pawn>(UGameplayStatics::GetActorOfClass(GetWorld(), APacman_Pawn::StaticClass()));
		if (IsValid(PlayerChar)) {
			PlayerChar->SetActorLocation(FVector(10000, 10000, 0));
			if (IsValid(Sound))UGameplayStatics::SpawnSound2D(GetWorld(), Sound, 1, 1);

			FTimerHandle UnUsedHandle;
			GetWorldTimerManager().SetTimer(UnUsedHandle, this, &AEnemy::RestartWorld, 5.5, false);
		}
	}
}

void AEnemy::Hunt()
{

	APacman_Pawn* PlayerChar=Cast<APacman_Pawn>( UGameplayStatics::GetActorOfClass(GetWorld(), APacman_Pawn::StaticClass()));
	if (IsValid(PlayerChar)) {
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, "start chasing player");
		EnemyAIController->MoveToActor(PlayerChar, -1, true);
	}
	else GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "Didn't found player's character");
}

void AEnemy::Idle()
{
	GameFinished = true;
	State = EEnemyState::Idle;
	EnemyAIController->StopMovement();
}
void AEnemy::RestartWorld() {
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
};