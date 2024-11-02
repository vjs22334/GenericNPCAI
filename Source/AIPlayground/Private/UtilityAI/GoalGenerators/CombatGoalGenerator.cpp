// Fill out your copyright notice in the Description page of Project Settings.


#include "UtilityAI/GoalGenerators/CombatGoalGenerator.h"

#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"

#include "UtilityAI/GoalGenerators/CombatGoalData.h"
#include "UtilityAI/GoalGenerators/GoalGeneratorComponent.h"

void UCombatGoalGenerator::PerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	// started seeing a target player
	if (Stimulus.WasSuccessfullySensed())
	{
		//Lets not switch targets for now
		if (M_CombatTargetActor == nullptr)
		{
			if (Stimulus.Type == UAISense::GetSenseID<UAISense_Sight>())
			{
				// cache the actor
				M_CombatTargetActor = Actor;
				// create and push combat goal
				M_CombatGoalData = NewObject<UCombatGoalData>(this);
				M_CombatGoalData->TargetActor = M_CombatTargetActor;

				M_GoalGeneratorComponent->AddGoal(M_CombatGoalData);
				// remove search area goal if any
				if (M_SearchGoalData!=nullptr)
				{
					M_GoalGeneratorComponent->RemoveGoal(M_SearchGoalData);
				}
		
			}
		}
	}
	else
	{
		if (Actor == M_CombatTargetActor)
		{
			// stopped seeing a target player
			M_CombatTargetActor = nullptr;
		
			if (M_CombatGoalData!=nullptr)
			{
				M_GoalGeneratorComponent->RemoveGoal(M_CombatGoalData);
				M_CombatGoalData = nullptr;
			}

			if (M_SearchGoalData == nullptr)
			{
				M_SearchGoalData = NewObject<USearchGoalData>(this);
				M_SearchGoalData->Location = Stimulus.StimulusLocation;

				M_GoalGeneratorComponent->AddGoal(M_SearchGoalData);
			}
		}
		
	}

	
}


void UCombatGoalGenerator::SetupPerception(AActor* goalOwner)
{
	AAIController* controller = Cast<AAIController>(Cast<APawn>(goalOwner)->GetController());
	if (controller)
	{
		UAIPerceptionComponent* PerceptionComponent = controller->GetPerceptionComponent();
		if (PerceptionComponent)
		{
			PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &UCombatGoalGenerator::PerceptionUpdated);
			M_PerceptionSetupSuccessful = true;
		}
	}
}

void UCombatGoalGenerator::EvaluateGoal_Implementation(AActor* goalOwner)
{
	Super::EvaluateGoal_Implementation(goalOwner);
	//TODO: should move this to the initialize method. Its here due to not being sure of the onpossess call order before which the ai controller will be null
	if(!M_PerceptionSetupSuccessful)
	{
		SetupPerception(goalOwner);
	}
}

void UCombatGoalGenerator::Initialize_Implementation(AActor* goalOwner, UGoalGeneratorComponent* owner)
{
	Super::Initialize_Implementation(goalOwner, owner);
	M_GoalOwner = goalOwner;
	M_GoalGeneratorComponent = owner;
}
