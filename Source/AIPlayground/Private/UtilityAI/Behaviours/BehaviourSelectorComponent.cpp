// Fill out your copyright notice in the Description page of Project Settings.


#include "UtilityAI/Behaviours/BehaviourSelectorComponent.h"

// Sets default values for this component's properties
UBehaviourSelectorComponent::UBehaviourSelectorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UBehaviourSelectorComponent::ChooseBehaviourToRun()
{
	M_NextChosenBehaviour = nullptr;
	M_NextChosenBehaviourGoalData = nullptr;
	
	float CurrentUtitlityScore = 0;

	for (UBaseBehaviour* priorityBehaviour : M_PriorityBehaviourRefs)
	{
		if(priorityBehaviour->CheckPreConditions())
		{
			UBaseGoalData* goalData = TryAndGetGoalDataClass(priorityBehaviour->GoalDataClass);
			if (priorityBehaviour->CanExecuteWithoutGoalData || goalData != nullptr)
			{
				if(priorityBehaviour->GetSelectionScore(goalData)>CurrentUtitlityScore)
				{
					CurrentUtitlityScore = priorityBehaviour->GetSelectionScore(goalData);
					M_NextChosenBehaviour = priorityBehaviour;
					M_NextChosenBehaviourGoalData = goalData;
				}
			}
			
		}
	}
	
	CurrentUtitlityScore = 0;
	for (UBaseBehaviour* behaviour : M_BehaviourRefs)
	{
		if(behaviour->CheckPreConditions())
		{
			UBaseGoalData* goalData = TryAndGetGoalDataClass(behaviour->GoalDataClass);
			if (behaviour->CanExecuteWithoutGoalData || goalData != nullptr)
			{
				if(behaviour->GetSelectionScore(goalData)>CurrentUtitlityScore)
				{
					CurrentUtitlityScore = behaviour->GetSelectionScore(goalData);
					M_NextChosenBehaviour = behaviour;
					M_NextChosenBehaviourGoalData = goalData;
				}
			}
		}
	}
	
	// iterate through behaviour list and cross check with goal data to create runnable behaviours list
	// Iterate through runnable behaviours list and calculate utility scores
	// set behaviour with highest utility score as behaviour to run
	
}

void UBehaviourSelectorComponent::SwitchToNewBehaviour()
{
	if (M_NextChosenBehaviour == nullptr)
	{
		return;
	}
	//call exit of current behaviour
	//start new behaviour
	if(M_CurrentBehaviour!=nullptr)
	{
		M_CurrentBehaviour->BehaviourExit();
	}
	M_CurrentBehaviour = M_NextChosenBehaviour;
	M_CurrentBehaviourGoalData = M_NextChosenBehaviourGoalData;
	M_CurrentBehaviour->BehaviourEnter(M_CurrentBehaviourGoalData);
}

UBaseGoalData* UBehaviourSelectorComponent::TryAndGetGoalDataClass(TSubclassOf<UBaseGoalData> goalDataClass)
{
	for (int i = 0; i < M_CurrentGoalDatas.Num(); i++)
	{
		if(M_CurrentGoalDatas[i]->GetClass() == goalDataClass)
		{
			return M_CurrentGoalDatas[i];
		}
	}
	return nullptr;
}




// Called when the game starts
void UBehaviourSelectorComponent::BeginPlay()
{
	Super::BeginPlay();
	
	M_CurrentBehaviour = nullptr;
	M_NextChosenBehaviour = nullptr;
	M_GoalGeneratorComponentRef = Cast<UGoalGeneratorComponent>(GetOwner()->GetComponentByClass(UGoalGeneratorComponent::StaticClass()));
	AddTickPrerequisiteComponent(M_GoalGeneratorComponentRef);
	
	for(TSubclassOf<UBaseBehaviour> baseBehaviourClass : M_BehaviourList)
	{
		UBaseBehaviour* BaseBehaviour = NewObject<UBaseBehaviour>(this,baseBehaviourClass);
		BaseBehaviour->Initialize(GetOwner(),this);
		M_BehaviourRefs.Add(BaseBehaviour);
	}

	for(TSubclassOf<UBaseBehaviour> baseBehaviourClass : M_PriorityBehaviourList)
	{
		UBaseBehaviour* BaseBehaviour = NewObject<UBaseBehaviour>(this,baseBehaviourClass);
		BaseBehaviour->Initialize(GetOwner(),this);
		M_PriorityBehaviourRefs.Add(BaseBehaviour);
	}
	
	ChooseBehaviourToRun();
	if (M_NextChosenBehaviour != nullptr)
	{
		SwitchToNewBehaviour();
	}
	
}


// Called every frame
void UBehaviourSelectorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//Update goal datas 
	M_CurrentGoalDatas = M_GoalGeneratorComponentRef->GetCurrentGoalDatas();

	ChooseBehaviourToRun();
	
	if(M_CurrentBehaviour != nullptr)
	{
		if(M_CurrentBehaviour->GetBehaviourExecutionState() == BehaviourExecutionState::RUNNING)
		{
			if(M_NextChosenBehaviour != nullptr && M_NextChosenBehaviour != M_CurrentBehaviour && M_CurrentBehaviour->GetBehaviourInterruptibiliyState() == BehaviourInterruptibilityState::INTERRUPTIBLE)
			{
				SwitchToNewBehaviour();
			}
			else
			{
				M_CurrentBehaviour->BehaviourTick();

			}
		}
		else if(M_CurrentBehaviour->GetBehaviourExecutionState() == BehaviourExecutionState::COMPLETED)
		{
			SwitchToNewBehaviour();
		}
		else if(M_CurrentBehaviour->GetBehaviourExecutionState() == BehaviourExecutionState::FAILED)
		{
			//TODO: some error logging
			SwitchToNewBehaviour();
		}
	}
	else if (M_NextChosenBehaviour != nullptr)
	{
		SwitchToNewBehaviour();
	}
	
}

