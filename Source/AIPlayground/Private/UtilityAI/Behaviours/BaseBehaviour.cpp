// Fill out your copyright notice in the Description page of Project Settings.


#include "UtilityAI/Behaviours/BaseBehaviour.h"

bool UBaseBehaviour::CheckPreConditions_Implementation(UBaseGoalData* GoalData)
{
	return true;
}

void UBaseBehaviour::Initialize_Implementation(AActor* OwnerActor, UBehaviourSelectorComponent* owner)
{
	GoalOwner = OwnerActor;
	BehaviourSelectorRef = owner;
}

void UBaseBehaviour::BehaviourEnter_Implementation(UBaseGoalData* GoalData)
{
	if (GoalData == nullptr)
	{
		M_BehaviourState = BehaviourExecutionState::FAILED;
		return;
	}
}

void UBaseBehaviour::BehaviourTick_Implementation(float DeltaTime)
{
}

void UBaseBehaviour::BehaviourExit_Implementation()
{
}

float UBaseBehaviour::GetSelectionScore_Implementation(UBaseGoalData* GoalData)
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
