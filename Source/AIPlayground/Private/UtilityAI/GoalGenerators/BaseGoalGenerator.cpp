// Fill out your copyright notice in the Description page of Project Settings.


#include "UtilityAI/GoalGenerators/BaseGoalGenerator.h"

void UBaseGoalGenerator::EvaluateGoal_Implementation(AActor* goalOwner, float DeltaTime)
{
}

void UBaseGoalGenerator::Initialize_Implementation(AActor* goalOwner, UGoalGeneratorComponent* owner)
{
	
}

UWorld* UBaseGoalGenerator::GetWorld() const
{
	UObject *outer = GetOuter();
	if (outer && outer->IsA<AActor>() && !outer->HasAnyFlags(RF_ClassDefaultObject))
	{
		return outer->GetWorld();
	}

	return nullptr;
}
	

