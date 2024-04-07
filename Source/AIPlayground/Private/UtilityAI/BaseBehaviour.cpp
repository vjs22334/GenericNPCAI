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

void UBaseBehaviour::BehavourTick(UBaseGoalData* GoalData)
{
}

float UBaseBehaviour::GetSelectionScore()
{
	return 0;
}
