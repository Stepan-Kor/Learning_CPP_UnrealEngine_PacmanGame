// Fill out your copyright notice in the Description page of Project Settings.
#include "FoodActor.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Engine/EngineTypes.h"
#include "Components/StaticMeshComponent.h"
#include "Pacman_Pawn.h"
//class APacman_Pawn;
// Sets default values
AFoodActor::AFoodActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Scene Component"));
	RootComponent = SceneComponent;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	Mesh->SetRelativeScale3D(FVector(0.2, 0.2, 0.2));
	Mesh->SetRelativeLocation(FVector(0, 0, 75));
	Mesh->SetCollisionProfileName("OverlapAll");
	//ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("StaticMesh'/Game/Pacman/Meshes/SM_FruitCherry.SM_FruitCherry'"));
	//Material'/Game/Pacman/Materials/M_PlainRed.M_PlainRed';

	static ConstructorHelpers::FObjectFinder<UMaterial> FrMaterial(TEXT("Material'/Game/Pacman/Materials/M_PlainRed.M_PlainRed'"));
	if(FrMaterial.Succeeded())FruitMaterial = FrMaterial.Object;
	ConstructorHelpers::FObjectFinder<UStaticMesh>UpMeshAsset(TEXT("StaticMesh'/Game/Pacman/Meshes/SM_FruitCherry.SM_FruitCherry'"));
	if (UpMeshAsset.Succeeded()) {
		StaticMeshFruit = UpMeshAsset.Object;
		TestStaticMesh = UpMeshAsset.Object;
		//GEngine->AddOnScreenDebugMessage(-1, 20, FColor::Emerald, StaticMeshFruit->GetName());
		//GEngine->AddOnScreenDebugMessage(-1, 20, FColor::Emerald, TestStaticMesh->GetName());
	}
	
	if (Food_Type == EFoodType::Regular) {
		ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("StaticMesh'/Engine/EngineMeshes/Sphere.Sphere'"));
		if (MeshAsset.Succeeded()) {
			//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Green, "Food Mesh is ok");
			Mesh->SetStaticMesh(MeshAsset.Object);

			static ConstructorHelpers::FObjectFinder<UMaterial> Material(TEXT("Material'/Game/Pacman/Materials/M_Foodie_Regular.M_Foodie_Regular'"));
			//	UMaterialInterface* Materail =FoundMesh
			if (Material.Succeeded())Mesh->SetMaterial(0, Material.Object);
			else GEngine->AddOnScreenDebugMessage(-1, 45, FColor::Red, "Food material not found");
		}
	}
	
	//SoundCue'/Game/Pacman/Audio/AS_EatDotCue.AS_EatDotCue'
	static ConstructorHelpers::FObjectFinder<USoundCue> ConsumptionCue(TEXT("SoundCue'/Game/Pacman/Audio/AS_EatDotCue.AS_EatDotCue'"));
	// Store a reference to the Cue asset - we'll need it later.
	if(ConsumptionCue.Succeeded())	ConsumptionSound = ConsumptionCue.Object;
}

// Called when the game starts or when spawned
void AFoodActor::BeginPlay()
{
	Super::BeginPlay();
	if (Food_Type == EFoodType::PowerUp) {
		//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Emerald, UEnum::GetValueAsString(Food_Type)); 
		if (IsValid(Mesh)) {
			Mesh->SetStaticMesh(StaticMeshFruit);
			FString string = "Mesh is ";
			string.Append(StaticMeshFruit->GetName());
			Mesh->SetMaterial(0, FruitMaterial);
			//GEngine->AddOnScreenDebugMessage(-1, 20, FColor::Emerald, string);
			//GEngine->AddOnScreenDebugMessage(-1, 20, FColor::Emerald, TestStaticMesh->GetName());
		}
	}
	Mesh->OnComponentBeginOverlap.AddDynamic(this, &AFoodActor::OnBeginOverlap);

}
// Called every frame
void AFoodActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
bool AFoodActor::TestFunction(FString Message)
{
	return false;
}
void AFoodActor::Consume()
{
	UGameplayStatics::PlaySound2D(this, ConsumptionSound,2);
	FoodConsumedEvent.Broadcast(Food_Type);
	Destroy();
}

//FComponentBeginOverlapSignature
void AFoodActor::OnBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bfromSweep, const FHitResult& SweepResult)
{
	if (IsValid(OtherActor)) {
		//OtherActor->Execute_FoodOverlaped();
		IFoodInterface* TempFoodInterface = Cast<IFoodInterface>(OtherActor);
		if (TempFoodInterface!=nullptr) 
		{
			TempFoodInterface->Execute_FoodOverlaped(OtherActor);
			TempFoodInterface->TestCppFunction();

		}



		APacman_Pawn* LocalPacman = Cast<APacman_Pawn>(OtherActor);
		if (IsValid(LocalPacman)) {
			
			//LocalPacman->Execute_FoodOverlaped(LocalPacman);
			if (Food_Type == EFoodType::PowerUp) {
				LocalPacman->ChangeSpeed(true);
				//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Magenta, "SPEEDBOOST");

			}
			Consume();
		}
	}
}
