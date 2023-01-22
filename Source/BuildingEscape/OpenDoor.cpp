// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "Components/PrimitiveComponent.h"
#include "Gameframework/Actor.h"
#include "engine/World.h"
#include "Math/Vector.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner();
	playerOpens = GetWorld()->GetFirstPlayerController()->GetCharacter();
	PressurePlateError();
	
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// poll the trigger volume 
	if (GetTotalMassOfActorsOnPlate()>TriggerMass) {
		 
		OnOpen.Broadcast();
	}else{
		OnClose.Broadcast();
	}
}


float UOpenDoor::GetTotalMassOfActorsOnPlate() {

	float TotalMass = 0.0f;
	//Find all the overlapping actors
	TArray<AActor*> OverLappingActors;
	
	if (!PressurePlate) {return TotalMass;}
	PressurePlate->GetOverlappingActors(OUT OverLappingActors);
	
	//Iterate Through them Adding masses
	for (const auto& Actor: OverLappingActors) {
	
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		//UE_LOG(LogTemp, Warning, TEXT("%s on pressure plate"), *Actor->GetName());
	}
	
	return TotalMass;
}

void UOpenDoor::PressurePlateError() {
	
	if (PressurePlate == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("Error Missing PressurePlate trigger in %s!"), *GetOwner()->GetName());
	}
}