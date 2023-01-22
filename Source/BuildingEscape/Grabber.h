// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include"PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/InputComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
private:
	//how far ahead of the player can we reach in 
	float Reach = 100.0f;
	
	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent* InputComponent = nullptr;

	//Ray-Cast and grab what's in reach
	void Grab();

	//called when grab when grab is released
	void release();

	//find attach physics
	void FindPhysicsHandleComponent();

	//setup (assumed) attached input component
	void SetupInputComponent();

	//return hit for first physics body in reach
	const FHitResult GetFirstPhysicsBodyInReach();

	//returns the start of the reah lines
	FVector GetReachLineStart();

	//returns current lines of reach
	FVector GetReachLineEnd();
};
