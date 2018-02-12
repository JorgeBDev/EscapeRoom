// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DrawDebugHelpers.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
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

	float raycastReach = 200.0f;
	UPhysicsHandleComponent* physicsHandle = nullptr;
	UInputComponent* inputComponent = nullptr;

	//raycast and grab
	void Grab();

	//called when grab key is release
	void Release();

	//Find attached PhysicsHandleComponent (only appears at runtime)
	void FindPhysicsHandleComponent();

	//Find attached InputComponent (only appears at runtime)
	void FindInputComponentAndSetActions();

	//GetLineTraceEnd
	FVector GetLineTraceEnd();

	//Return hit of physics body in reach
	FHitResult GetPhysicsBodyInReach() const;
};
