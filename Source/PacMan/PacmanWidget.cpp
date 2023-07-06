// Fill out your copyright notice in the Description page of Project Settings.


#include "PacmanWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "FoodActor.h"
#include "Enemy.h"
#include "Pacman_Pawn.h"
#include "Sound/SoundCue.h"
#include "SaveFile.h"

bool UPacmanWidget::Initialize()
{	
	Super::Initialize();
	if(IsValid(StartGame))StartGame->OnClicked.AddDynamic(this, &UPacmanWidget::StartGameButtonClicked);
	if(IsValid(TextBlockPoints))	TextBlockPoints->SetText(FText::FromString(FString::FromInt(0)));
	if (IsValid(TextBlockMaxPoints))TextBlockMaxPoints->SetText(FText::FromString(FString::FromInt(12)));
	TArray <AActor*> FoundFood;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFoodActor::StaticClass(), FoundFood);
	AFoodActor* FoodActor;
	for (AActor* FoundActor : FoundFood) {
		FoodActor = Cast<AFoodActor>(FoundActor);
		if (IsValid(FoodActor))FoodActor->FoodConsumedEvent.AddDynamic(this, &UPacmanWidget::SomeFoodConsumed);
	}

	if (IsValid(LoadedGame = Cast<USaveFile>(UGameplayStatics::LoadGameFromSlot(TEXT("MainSlot"), 0)))) {
		MaxPoints = LoadedGame->MaxPoints;
	}
	else { 
		LoadedGame = Cast<USaveFile>(UGameplayStatics::CreateSaveGameObject(USaveFile::StaticClass()));
		if (LoadedGame)UGameplayStatics::SaveGameToSlot(LoadedGame, "MainSlot", 0);
		else GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Orange, "Didnt create loaded file");
	}
	if (IsValid(TextBlockMaxPoints))TextBlockMaxPoints->SetText(FText::FromString(FString::FromInt(MaxPoints)));


	return true;
}

void UPacmanWidget::StartGameButtonClicked()
{
	GEngine->AddOnScreenDebugMessage(-1, 6, FColor::Orange, "Start Game button pressed.");
}

void UPacmanWidget::PacmanWidget()
{	
	//static ConstructorHelpers::FObjectFinder<USoundCue>DeathSound(TEXT("SoundCue'/Game/Pacman/Audio/AS_DeathCue.AS_DeathCue'"));
	static ConstructorHelpers::FObjectFinder<USoundCue> FinishSoundAsset(TEXT("SoundCue'/Game/Pacman/Audio/AM_GameEndedVictoryCue.AM_GameEndedVictoryCue'"));
	if (FinishSoundAsset.Succeeded())SuccessSoundCue = FinishSoundAsset.Object;
}

void UPacmanWidget::RestartGame()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName(GetWorld()->GetName()), false);
}


void UPacmanWidget::SomeFoodConsumed(EFoodType TypeofFood)
{
	//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Orange, "foodconsumed");
	int Increasing;
	if (TypeofFood == EFoodType::Regular) Increasing = 1;
	else Increasing = 10;
	FoodEaten+=Increasing;
	if (FoodEaten > MaxPoints) {
		MaxPoints = FoodEaten;
		TextBlockMaxPoints->SetText(FText::FromString(FString::FromInt(MaxPoints)));
		if (IsValid(LoadedGame)) {
			LoadedGame->MaxPoints = MaxPoints;
			UGameplayStatics::SaveGameToSlot(LoadedGame, "MainSlot", 0);
		}
	}
	if (IsValid(TextBlockPoints)) 	TextBlockPoints->SetText(FText::FromString(FString::FromInt(FoodEaten)));
	//Maxpoints 247
	if (FoodEaten > 246 && !GameFinished) {
		GameFinished = true;
		AEnemy* Ghost =Cast<AEnemy>(UGameplayStatics::GetActorOfClass(GetWorld(), AEnemy::StaticClass()));
		if (IsValid(Ghost))Ghost->Idle();
		APacman_Pawn* PlayerPawn = Cast<APacman_Pawn>(UGameplayStatics::GetActorOfClass(GetWorld(), APacman_Pawn::StaticClass()));
		if (IsValid(PlayerPawn))PlayerPawn->GameFinished = true;
		if (SuccessSoundCue!=nullptr)UGameplayStatics::SpawnSound2D(GetWorld(), SuccessSoundCue);
		FTimerHandle UnUsedHandle;
		GetWorld()->GetTimerManager().SetTimer(UnUsedHandle, this, &UPacmanWidget::RestartGame, 7.7, false);
		
	}
	
}

