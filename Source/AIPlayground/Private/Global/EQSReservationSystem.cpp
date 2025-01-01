// Fill out your copyright notice in the Description page of Project Settings.


#include "EQSReservationSystem.h"

bool UEQSReservationSystem::ReserveCoverLocation(FVector location)
{
	if (! M_ReservedCoverLocations.Contains(location))
	{
		M_ReservedCoverLocations.Add(location);
		return true;
	}
	return false;
}

TArray<FVector> UEQSReservationSystem::GetReserveCoverLocations()
{
	return M_ReservedCoverLocations;
}

bool UEQSReservationSystem::UnReserveCoverLocation(FVector location)
{
	if (M_ReservedCoverLocations.Contains(location))
	{
		M_ReservedCoverLocations.Remove(location);
		return true;
	}
	return false;
}

bool UEQSReservationSystem::ReserveFiringLocation(FVector location)
{
	if (!M_ReservedFiringLocations.Contains(location))
	{
		M_ReservedFiringLocations.Add(location);
		return true;
	}
	return false;
}

TArray<FVector> UEQSReservationSystem::GetReservedFiringLocations()
{
	return M_ReservedFiringLocations;
}

bool UEQSReservationSystem::UnReserveFiringLocation(FVector location)
{
	if (M_ReservedCoverLocations.Contains(location))
	{
		M_ReservedCoverLocations.Remove(location);
		return true;
	}
	return false;
}
