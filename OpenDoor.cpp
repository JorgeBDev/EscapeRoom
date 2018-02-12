// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "Gameframework/Actor.h"
#include "Engine/World.h"
#include "Components/PrimitiveComponent.h"

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

	// ...
	//es: linea de abajo consigue el pawn que tiene el first player controller
	owner = GetOwner();
	if (!pressurePlate) {
		UE_LOG(LogTemp, Error, TEXT("pressurePlate couldn't be found in: %s"), *GetOwner()->GetName());
	}
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Poll the trigger volume
	if (GetTotalMassOfActorsOnPlate() > 35.f) { //mkae into a parameter
		
		OnOpen.Broadcast();
		//lastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}
	else {
		OnClose.Broadcast();
	}

	//check if it's time to close the door
	/*
	if (GetWorld()->GetTimeSeconds() >= (lastDoorOpenTime + doorCloseDelay)) {

		CloseDoor();
	}*/
	

	//UE_LOG(LogTemp, Warning, TEXT(" The running time is: %f"), GetWorld()->GetTimeSeconds());
}

float UOpenDoor::GetTotalMassOfActorsOnPlate() {
	
	float totalMass = 0.f;
	
	//find overlapping Actors
	TArray<AActor*> overlappingActors;
	if (!pressurePlate) { return totalMass; } // es: devolvemos totalmass porque es lo que la funcion pide. Sino no podemos compilar
	pressurePlate->GetOverlappingActors(overlappingActors);

	//find overlapping actors
	for (const auto* actorInPlate : overlappingActors) {
		
		float mass = actorInPlate->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("%s mass is %f"), *actorInPlate->GetName(), mass);
		totalMass += actorInPlate->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("total mass is %f"), totalMass);
	}

	return totalMass;
}

