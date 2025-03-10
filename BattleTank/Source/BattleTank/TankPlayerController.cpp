// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTank.h"
#include "TankPlayerController.h"

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();
	auto PossessededTank = GetControlledTank();
	if( PossessededTank ){
		UE_LOG(LogTemp, Warning, TEXT("Controlling %s"), *(PossessededTank->GetName()));
	}else{
		UE_LOG(LogTemp, Error, TEXT("Missing tank"));
	}
}


// Called every frame
void ATankPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AimTowardsCrosshair();
}



ATank* ATankPlayerController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
}

void ATankPlayerController::AimTowardsCrosshair()
{
	if (!GetControlledTank()) return;

	FVector HitLocation; //Out parameter

	if( GetSightRayHitLocation(HitLocation) )
	{
		GetControlledTank()->AimAt(HitLocation);
			UE_LOG(LogTemp, Warning, TEXT("Hit Location: %s"), *HitLocation.ToString());
	}
}

// get world location of linetrace through crosshair, true if hits landscape
bool ATankPlayerController::GetSightRayHitLocation( FVector& HitLocation) const{
	
	//find crosshair position in pixel coordinates
	int32 ViewPortSizex, ViewPortSizey;
	GetViewportSize(ViewPortSizex,ViewPortSizey);
	auto ScreenLocation = FVector2D(ViewPortSizex*CrossHairXLocation, ViewPortSizey*CrossHairYLocation);

	//de project the screen position of the crossahir to a world direction
	FVector LookDirection;
	if( GetLookDirection(ScreenLocation, LookDirection) ){
		// Line trace along that look direction and see if we hit something
		GetLookVectorHitLocation(LookDirection, HitLocation); 
		UE_LOG(LogTemp, Warning, TEXT("Look direction: %s"), *LookDirection.ToString());
	}
	return true;
}

bool ATankPlayerController::GetLookDirection(FVector2D ScreenLocation, FVector& LookDirection) const {
	FVector CameraWorldDirection;
	return DeprojectScreenPositionToWorld(
		ScreenLocation.X, 
		ScreenLocation.Y, 
		CameraWorldDirection,
		LookDirection
		);
}

bool ATankPlayerController::GetLookVectorHitLocation(FVector LookDirection, FVector& HitLocation) const {

	FHitResult HitResult;
	auto StartLocation = PlayerCameraManager->GetCameraLocation();
	auto EndLocation = StartLocation + (LineTraceRange*LookDirection);

	if (GetWorld()->LineTraceSingleByChannel(
		HitResult,
		StartLocation,
		EndLocation,
		ECollisionChannel::ECC_Visibility)
	){
		HitLocation = HitResult.Location;
		return true;
	}
	HitLocation = FVector(0.f);
	return false;
}

