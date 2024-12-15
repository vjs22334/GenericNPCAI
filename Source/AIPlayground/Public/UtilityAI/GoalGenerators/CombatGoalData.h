// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseGoalData.h"
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
	TArray<AActor*> Targets;
};
