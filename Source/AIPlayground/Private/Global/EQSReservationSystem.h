// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "EQSReservationSystem.generated.h"

/**
 * 
 */
UCLASS()
class AIPLAYGROUND_API UEQSReservationSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	bool ReserveCoverLocation(FVector location);
	UFUNCTION(BlueprintCallable)
	TArray<FVector> GetReserveCoverLocations();
	UFUNCTION(BlueprintCallable)
	bool UnReserveCoverLocation(FVector location);

	UFUNCTION(BlueprintCallable)
	bool ReserveFiringLocation(FVector location);
	UFUNCTION(BlueprintCallable)
	TArray<FVector> GetReservedFiringLocations();
	UFUNCTION(BlueprintCallable)
	bool UnReserveFiringLocation(FVector location);

private:
	TArray<FVector> M_ReservedCoverLocations;
	TArray<FVector> M_ReservedFiringLocations;
};
