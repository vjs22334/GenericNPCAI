// Fill out your copyright notice in the Description page of Project Settings.


#include "UtilityAI/BaseBehaviour.h"

bool UBaseBehaviour::CheckPreConditions()
{
	return true;
}

void UBaseBehaviour::Initialize(AActor* OwnerActor, UBehaviourSelectorComponent* owner)
{
}

void UBaseBehaviour::BehaviourEnter(UBaseGoalData* GoalData)
{
}

void UBaseBehaviour::BehaviourTick(UBaseGoalData* GoalData)
{
}

void UBaseBehaviour::BehaviourExit(UBaseGoalData* GoalData)
{
	
}

float UBaseBehaviour::GetSelectionScore(UBaseGoalData* GoalData)
{
	return 0;
}

BehaviourExecutionState UBaseBehaviour::GetBehaviourExecutionState()
{
	return M_BehaviourState;
}

BehaviourInterruptibilityState UBaseBehaviour::GetBehaviourInterruptibiliyState()
{
	return M_BehaviourInterruptibilityState;
}
