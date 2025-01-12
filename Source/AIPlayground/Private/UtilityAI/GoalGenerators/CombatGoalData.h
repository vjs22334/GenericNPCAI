// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UtilityAI/GoalGenerators/BaseGoalData.h"
#include "CombatGoalData.generated.h"

/**
 * 
 */
UCLASS()
class AIPLAYGROUND_API UCombatGoalData : public UBaseGoalData
{
	GENERATED_BODY()
	public:
	UPROPERTY()
	TArray<AActor*> SelfTargets;

	UPROPERTY()
	TArray<AActor*> TeamTargets;
	
};
