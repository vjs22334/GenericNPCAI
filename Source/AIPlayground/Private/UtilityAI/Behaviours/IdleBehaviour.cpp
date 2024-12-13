// Fill out your copyright notice in the Description page of Project Settings.


#include "UtilityAI/Behaviours/IdleBehaviour.h"

bool UIdleBehaviour::CheckPreConditions_Implementation(UBaseGoalData* GoalData)
{
	return true;
}

float UIdleBehaviour::GetSelectionScore_Implementation(UBaseGoalData* GoalData)
{
	return 1;
}

void UIdleBehaviour::BehaviourEnter_Implementation(UBaseGoalData* GoalData)
{
	Super::BehaviourEnter_Implementation(GoalData);
	M_BehaviourState = BehaviourExecutionState::COMPLETED;
	M_BehaviourInterruptibilityState = BehaviourInterruptibilityState::INTERRUPTIBLE;
}
