// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UtilityAI/GoalGenerators/BaseGoalData.h"
#include "CoverGoalData.generated.h"

/**
 * 
 */
UCLASS()
class AIPLAYGROUND_API UCoverGoalData : public UBaseGoalData
{
	GENERATED_BODY()
public:
	UPROPERTY()
	FVector CoverLocation;
};
