// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "Gameframework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Components/PrimitiveComponent.h"

#define OUT
// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	// ...
	FindPhysicsHandleComponent();
	FindInputComponentAndSetActions();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!physicsHandle) { return; }
	//if physics handle is attached
	if (physicsHandle->GrabbedComponent) {
		//move object we're holding
		physicsHandle->SetTargetLocation(GetLineTraceEnd());
	}
		
}

void UGrabber::Grab(){

	UE_LOG(LogTemp, Warning, TEXT("Grab Input Pressed"));

	///try and reach object
	
	//line trace if we hit smth then attached physics handle
	auto hitResult = GetPhysicsBodyInReach(); 
	auto componentToGrab = hitResult.GetComponent(); //es: esto coge el mesh del objeto. Tambien: componentToGrab es un componente de las fisicas que se encarga de coger los planos basicos para la colision 
	auto actorHit = hitResult.GetActor();
	
	//if we hit smth
	if (!physicsHandle) { return; }
	if (actorHit) {
		//Attach physicsHandle
		physicsHandle->GrabComponent(componentToGrab, NAME_None, 
			//es: componentToGrab es un componente de las fisicas que se encarga de coger los planos basicos para la colision 
			//es: NAME_None es porque no estamos utilizando Huesos que coger, es simplemente una primitiva
			componentToGrab->GetOwner()->GetActorLocation(), true); //es: el true hace que la rotation esté activada
	}
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab InputReleased"));
	if (!physicsHandle) { return; }
	//release physics handle
	physicsHandle->ReleaseComponent();
}

//find physicsHandle (only appears at runtime)
void UGrabber::FindPhysicsHandleComponent()
{
	physicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (physicsHandle == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("PhysicsComponent couldn't be found in: %s"), *GetOwner()->GetName());
	}
}

//find inputComponent (only appears at runtime)
void UGrabber::FindInputComponentAndSetActions()
{
	inputComponent = GetOwner()->FindComponentByClass < UInputComponent>();

	if (inputComponent) {
		//Bind Input
		inputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		inputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else {

		UE_LOG(LogTemp, Error, TEXT("InputComponent couldn't be found in: %s"), *GetOwner()->GetName());
	}
}

FVector UGrabber::GetLineTraceEnd()
{
	FVector playerViewPointLocation;
	FRotator playerViewPointRotator;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint( //es: ese out lo hemos definido nosotros para remarcar que los parametros han sido cambiado de valor
		OUT playerViewPointLocation,
		OUT playerViewPointRotator);
	FVector linetraceEnd = playerViewPointLocation + playerViewPointRotator.Vector() * raycastReach; //es: esta crea un raycast de hacia donde miremos
	return FVector(linetraceEnd);
}

FHitResult UGrabber::GetPhysicsBodyInReach() const
{
	FVector playerViewPointLocation;
	FRotator playerViewPointRotator;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint( //es: ese out lo hemos definido nosotros para remarcar que los parametros han sido cambiado de valor
		OUT playerViewPointLocation,
		OUT playerViewPointRotator);

	//50 cm above the player's head
	//FVector lineEndTrace = playerViewPointLocation + FVector(0.f, 0.f, 50.f); // es: esta linea crea un debug raycast en vertical hacia arriba
	FVector linetraceEnd = playerViewPointLocation + playerViewPointRotator.Vector() * raycastReach; //es: esta crea un raycast de hacia donde miremos

	/*///draw a red trace in the world to visualise
	DrawDebugLine(GetWorld(), playerViewPointLocation, linetraceEnd,
		FColor(*COLOR_RED), false, 0.f, 0.f, 5.f);
		*/

	//setup parameters
	FCollisionQueryParams traceParameters = (FName(TEXT("")), false, GetOwner());

	///Draw Raycast
	FHitResult hitResult;
	GetWorld()->LineTraceSingleByObjectType(hitResult, playerViewPointLocation,
		linetraceEnd,FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), traceParameters);

	//see what we hit
	AActor* actorHit = hitResult.GetActor();
	if (actorHit) { //es: Si no pones el if actor, da error 
		UE_LOG(LogTemp, Warning, TEXT("Object traced is: %s"), *actorHit->GetName());
	}

	return hitResult;
}

