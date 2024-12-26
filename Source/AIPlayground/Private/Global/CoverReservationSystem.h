// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CoverReservationSystem.generated.h"

/**
 * 
 */
UCLASS()
class AIPLAYGROUND_API UCoverReservationSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	bool ReserveCoverLocation(FVector location);
	UFUNCTION(BlueprintCallable)
	TArray<FVector> GetReserveCoverLocations();
	UFUNCTION(BlueprintCallable)
	bool UnReserveCoverLocation(FVector location);

private:
	TArray<FVector> M_ReservedCoverLocations;
};
