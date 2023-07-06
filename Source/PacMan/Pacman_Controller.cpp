// Fill out your copyright notice in the Description page of Project Settings.


#include "Pacman_Controller.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy.h"
#include "PacmanWidget.h"

//APacman_Controller::APacman_Controller() {
//
//	//static ConstructorHelpers::FClassFinder<UPacmanWidget>WidgetClassFounded(TEXT("WidgetBlueprint'/Game/BP_Pacman_Widget.BP_Pacman_Widget'"));
//	//if (WidgetClassFounded.Succeeded())PlayerHudClass = WidgetClassFounded.Class;
//	//PlayerHudClass = UPacmanWidget::StaticClass();
//}
void APacman_Controller::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction("MoveUp", IE_Pressed, this, &APacman_Controller::MoveUp);
	InputComponent->BindAction("MoveRight", IE_Pressed, this, &APacman_Controller::MoveRight);
	InputComponent->BindAction("MoveLeft", IE_Pressed, this, &APacman_Controller::MoveLeft);
	InputComponent->BindAction("MoveDown", IE_Pressed, this, &APacman_Controller::MoveDown);
	InputComponent->BindAction("Start", IE_Pressed, this, &APacman_Controller::StartGame);
	InputComponent->BindAction("ExitGame", IE_Pressed, this, &APacman_Controller::ExitGame);
	Pac_Pawn = Cast<APacman_Pawn>(GetPawn()); 
	
}

void APacman_Controller::BeginPlay()
{
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Orange, "pacman controller start");
	Pac_Pawn = Cast<APacman_Pawn>(GetPawn());
	bool WidgetCreated = false;
	MoveDown();

	if (IsValid(PlayerHudClass)) {
		PlayerHudWidget =Cast<UPacmanWidget>( CreateWidget(GetWorld(), PlayerHudClass));
		if (IsValid(PlayerHudWidget)) {
			PlayerHudWidget->AddToViewport();
			WidgetCreated = true;
		}
	}
	if(!WidgetCreated) GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Orange, "pacman controller didnt create widget");
}
void APacman_Controller::MoveUp()
{	if(IsValid(Pac_Pawn))
	Pac_Pawn->Change_Direction(FVector(0,-1,0));
	else GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "controller dont have pac man pawn");
}

void APacman_Controller::MoveDown()
{
	if (IsValid(Pac_Pawn))
		Pac_Pawn->Change_Direction(FVector(0, 1, 0));
	else GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "controller dont have pac man pawn");
}

void APacman_Controller::MoveLeft()
{
	if (IsValid(Pac_Pawn))
	Pac_Pawn->Change_Direction(FVector(-1, 0, 0));
	else GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "controller dont have pac man pawn");
}

void APacman_Controller::MoveRight()
{
	if (IsValid(Pac_Pawn))
	Pac_Pawn->Change_Direction(FVector(1,0, 0));
	else GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "controller dont have pac man pawn");
}

void APacman_Controller::StartGame()
{
	if (IsValid(Pac_Pawn)) {
		Pac_Pawn->Started = true;
		TArray <AActor*> Enemies;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), Enemies);
		for (AActor* Enemy : Enemies) {
			Cast<AEnemy>(Enemy)->Hunt();

		}
	}
}

void APacman_Controller::ExitGame()
{
	TEnumAsByte<EQuitPreference::Type> QuitPreference = EQuitPreference::Quit;
	UKismetSystemLibrary::QuitGame(GetWorld(), this, QuitPreference, true);
	
}

