// Fill out your copyright notice in the Description page of Project Settings.


#include "UtilityAI/GoalGenerators/CombatGoalGenerator.h"

#include "AIController.h"
#include "Global/DetectionSystem.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"

#include "UtilityAI/GoalGenerators/CombatGoalData.h"
#include "UtilityAI/GoalGenerators/GoalGeneratorComponent.h"


void UCombatGoalGenerator::SetupPerception(AActor* goalOwner)
{
	M_DetectionComponent = Cast<UDetectionComponent>(goalOwner->GetComponentByClass(UDetectionComponent::StaticClass()));
	M_PerceptionSetupSuccessful = true;
}

void UCombatGoalGenerator::EvaluateGoal_Implementation(AActor* goalOwner, float DeltaTime)
{
	Super::EvaluateGoal_Implementation(goalOwner, DeltaTime);
	//TODO: should move this to the initialize method. Its here due to not being sure of the onpossess call order before which the ai controller will be null
	if(!M_PerceptionSetupSuccessful)
	{
		SetupPerception(goalOwner);
	}

	if (M_DetectionComponent!=nullptr)
	{
		TArray<AActor*> selfVisibleActors = M_DetectionComponent->GetAllVisibleActorsNotOfTeam(M_DetectionComponent->MyTeam);

		if (selfVisibleActors.Num() == 0)
		{
			// stopped seeing a target player
			if (M_CombatGoalData!=nullptr)
			{
				M_CombatGoalData->Targets.Empty(); //let the behaviour know the target is lost
				M_GoalGeneratorComponent->RemoveGoal(M_CombatGoalData);
				M_CombatGoalData = nullptr;
			}

			if (M_SearchGoalData == nullptr && M_CombatTargetActor != nullptr)
			{
				M_SearchGoalData = NewObject<USearchGoalData>(this);
				M_SearchGoalData->Location = M_CombatTargetActor->GetActorLocation();
				M_GoalGeneratorComponent->AddGoal(M_SearchGoalData);
				M_TimeElapsedSinceSearchStart = 0;
			}
		}
		else
		{
			if (M_CombatGoalData == nullptr)
			{
				// create and push combat goal
				M_CombatGoalData = NewObject<UCombatGoalData>(this);

				M_GoalGeneratorComponent->AddGoal(M_CombatGoalData);
				// remove search area goal if any
				if (M_SearchGoalData!=nullptr)
				{
					M_GoalGeneratorComponent->RemoveGoal(M_SearchGoalData);
					M_SearchGoalData = nullptr;
				}
			}
			M_CombatGoalData->Targets = selfVisibleActors;
			M_CombatTargetActor = selfVisibleActors[0];
		}
		
	}

	if (M_SearchGoalData != nullptr)
	{
		M_TimeElapsedSinceSearchStart += DeltaTime;
		if (M_TimeElapsedSinceSearchStart >= M_SearhGoalExpiryTime)
		{
			M_GoalGeneratorComponent->RemoveGoal(M_SearchGoalData);
			M_SearchGoalData = nullptr;
			M_TimeElapsedSinceSearchStart = 0;
		}
	}
}

void UCombatGoalGenerator::Initialize_Implementation(AActor* goalOwner, UGoalGeneratorComponent* owner)
{
	Super::Initialize_Implementation(goalOwner, owner);
	M_GoalOwner = goalOwner;
	M_GoalGeneratorComponent = owner;
}
