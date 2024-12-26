// Fill out your copyright notice in the Description page of Project Settings.


#include "CoverReservationSystem.h"

bool UCoverReservationSystem::ReserveCoverLocation(FVector location)
{
	if (! M_ReservedCoverLocations.Contains(location))
	{
		M_ReservedCoverLocations.Add(location);
		return true;
	}
	return false;
}

TArray<FVector> UCoverReservationSystem::GetReserveCoverLocations()
{
	return M_ReservedCoverLocations;
}

bool UCoverReservationSystem::UnReserveCoverLocation(FVector location)
{
	if (M_ReservedCoverLocations.Contains(location))
	{
		M_ReservedCoverLocations.Remove(location);
		return true;
	}
	return false;
}
