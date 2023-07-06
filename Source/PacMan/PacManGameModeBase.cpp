// Copyright Epic Games, Inc. All Rights Reserved.


#include "PacManGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"
//class ACameraActor;

void APacManGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	//GEngine->AddOnScreenDebugMessage(-1, 20, FColor::Yellow, "Mode started");

	/*GetWorld()->GetAllactorsofclass*/
		
		//AActor TestActor;
	//	ACameraActor::GetClass();
		TSubclassOf<AActor> ClassToFind= ACameraActor::StaticClass();
		//ClassToFind= APacManGameModeBase::StaticClass();
		//ClassToFind= ACameraActor::StaticClass();
	//	TSubclassOf<ACameraActor> ClassToFind;
		//class ACameraActor* ClassToFind;
		/*
		if (IsValid(ClassToFind)) {
			GEngine->AddOnScreenDebugMessage(-1, 20, FColor::Yellow, "classtofind:");
			GEngine->AddOnScreenDebugMessage(-1, 20, FColor::Yellow, ClassToFind->GetName());
		}
		else {GEngine->AddOnScreenDebugMessage(-1, 20, FColor::Yellow, "classtofind is empty");}
		TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ClassToFind, FoundActors);
	
	int i = 0;
	for (AActor* FoundedActor : FoundActors) {

		GEngine->AddOnScreenDebugMessage(-1, 20, FColor::Purple,FString::FromInt(i));
		i++;
	}*/
	//Cast<ACameraActor>();
	ACameraActor* TopDownCamera = Cast<ACameraActor>(UGameplayStatics::GetActorOfClass(GetWorld(), ClassToFind));
	//TopDownCamera->BecomeViewTarget(GEngine->GetFirstLocalPlayerController(GetWorld()));
	
	//GEngine->AddOnScreenDebugMessage(-1, 20, FColor::Red, TopDownCamera->GetName());
	GEngine->GetFirstLocalPlayerController(GetWorld())->SetViewTargetWithBlend(TopDownCamera, 1);

}
