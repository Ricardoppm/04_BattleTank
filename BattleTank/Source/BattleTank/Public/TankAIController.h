// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tank.h"
#include "AIController.h"
#include "TankAIController.generated.h"

/**
 * 
 */
UCLASS()
class BATTLETANK_API ATankAIController : public AAIController
{
	GENERATED_BODY()
	
private:

	virtual void Tick(float DeltaSeconds) override;

	ATank* GetControlledTank() const;

	void BeginPlay() override;

	ATank* GetPlayerTank() const;
	
};
