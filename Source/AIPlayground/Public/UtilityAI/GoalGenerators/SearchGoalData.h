// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseGoalData.h"
#include "SearchGoalData.generated.h"

/**
 * 
 */
UCLASS()
class AIPLAYGROUND_API USearchGoalData : public UBaseGoalData
{
	GENERATED_BODY()
public:
	UPROPERTY()
	FVector MoveToLocation = FVector::ZeroVector;
};
