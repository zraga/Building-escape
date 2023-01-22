///Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include"PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Components/InputComponent.h"
#include "Gameframework/Actor.h"
#include "engine/World.h"
#include "Runtime/Engine/Classes/Engine/EngineTypes.h"
#include "DrawDebugHelpers.h"
#include "Math/Vector.h"

#define OUT

/// Sets default values for this component's properties
UGrabber::UGrabber()
{
	/// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	/// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


///Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	///calling methods
	FindPhysicsHandleComponent();
	SetupInputComponent();

}

void UGrabber::FindPhysicsHandleComponent() {
	
	///look for attached physics handle
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("Error no PhysicsHAndle in %s!"), *GetOwner()->GetName());
	}
}

void UGrabber::SetupInputComponent(){

	///Look for attached input component
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	if (InputComponent) {

		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::release);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("%s InputComponent not there!"), *GetOwner()->GetName());
	}

}


void UGrabber::Grab() {
	
	///try and reach any actors with physics body collision channel set
	auto HitResults = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResults.GetComponent(); //gets the mesh in our case
	auto ActorHit = HitResults.GetActor();
	
	///if we hit something then attach a physics handle
	if (ActorHit) {
		
		if (!PhysicsHandle) { return; }
		
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			ComponentToGrab,//component to grab
			NAME_None, // no bones needed
			ComponentToGrab->GetOwner()->GetActorLocation(),//grab location 
			ComponentToGrab->GetOwner()->GetActorRotation() //grab rotation

		);

		
	}
}

void UGrabber::release() {
	
	if (!PhysicsHandle) { return; }
	PhysicsHandle->ReleaseComponent();

}


/// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (!PhysicsHandle) { return;}
	///if the physic handle is attached
	if (PhysicsHandle->GrabbedComponent) {
		/// move the object that we're holding!
		PhysicsHandle->SetTargetLocation(GetReachLineEnd());
	}

}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach() {
	
	///Draw a red trace in the world to visualise
	///DrawDebugLine(GetWorld(), GetReachLineStart(), GetReachLineEnd(), FColor(245, 125, 125), false, 0.0f, 0.0f, 10.0f);
	
	///Line-Trace(AKA ray-cast) out to reach distance
	FHitResult HitResults;
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(OUT HitResults,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	/*
		///see what we hit
		AActor* ActorHit = Hit.GetActor();
		///When the tracer collides with the object
		if (ActorHit) {

			UE_LOG(LogTemp, Warning, TEXT("Line trace hit: %s"), *(ActorHit->GetName()));

		}
	*/

	return HitResults;
}

FVector UGrabber::GetReachLineStart() {

	///needed or ot work =(
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);

	return PlayerViewPointLocation;

}

FVector UGrabber::GetReachLineEnd() {
	
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);
	
	return PlayerViewPointLocation + PlayerViewPointRotation.Vector()*Reach;

}