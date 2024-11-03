// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseBehaviour.h"
#include "IdleBehaviour.generated.h"

/**
 * 
 */
UCLASS()
class AIPLAYGROUND_API UIdleBehaviour : public UBaseBehaviour
{
	GENERATED_BODY()

public:
	virtual bool CheckPreConditions_Implementation() override;
	virtual float GetSelectionScore_Implementation(UBaseGoalData* GoalData) override;
	virtual void BehaviourEnter_Implementation(UBaseGoalData* GoalData) override;
};
