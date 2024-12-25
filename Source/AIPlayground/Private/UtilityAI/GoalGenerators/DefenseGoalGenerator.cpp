// Fill out your copyright notice in the Description page of Project Settings.


#include "UtilityAI/GoalGenerators/DefenseGoalGenerator.h"

#include "EnvironmentQuery/EnvQueryManager.h"
#include "UtilityAI/GoalGenerators/CoverGoalData.h"
#include "UtilityAI/GoalGenerators/GoalGeneratorComponent.h"


void UDefenseGoalGenerator::Initialize_Implementation(AActor* goalOwner, UGoalGeneratorComponent* owner)
{
	Super::Initialize_Implementation(goalOwner, owner);
	M_GoalOwner = goalOwner;
	M_GoalGeneratorComponent = owner;
	
}

void UDefenseGoalGenerator::HandleQueryResult(TSharedPtr<FEnvQueryResult> EnvQueryResult)
{
	if (EnvQueryResult->IsSuccsessful())
	{
		M_CoverGoalData->CoverLocation = EnvQueryResult->GetItemAsLocation(0);
	}
	M_ExecutingQuery = false;
}

void UDefenseGoalGenerator::EvaluateGoal_Implementation(AActor* goalOwner, float DeltaTime)
{
	if (M_CoverGoalData == nullptr)
	{
		// create and push defense goal
		M_CoverGoalData = NewObject<UCoverGoalData>(this);

		M_GoalGeneratorComponent->AddGoal(M_CoverGoalData);
	}

	if (!M_ExecutingQuery)
	{
		FEnvQueryRequest HidingSpotQueryRequest = FEnvQueryRequest(FindHidingSpotEQS, goalOwner);

		HidingSpotQueryRequest.Execute(
				EEnvQueryRunMode:: SingleResult, 
				this,    
				&UDefenseGoalGenerator::HandleQueryResult);
		M_ExecutingQuery = true;
	}
}
