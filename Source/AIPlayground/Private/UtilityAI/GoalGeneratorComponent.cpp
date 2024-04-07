// Fill out your copyright notice in the Description page of Project Settings.


#include "UtilityAI/GoalGeneratorComponent.h"

// Sets default values for this component's properties
UGoalGeneratorComponent::UGoalGeneratorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGoalGeneratorComponent::BeginPlay()
{
	Super::BeginPlay();

	for(TSubclassOf<UBaseGoalGenerator> goalGeneratorClass : M_GoalGenerators)
	{
		UBaseGoalGenerator* goalGenerator = NewObject<UBaseGoalGenerator>(this,goalGeneratorClass);
		goalGenerator->Initialize(GetOwner(),this);
		M_GoalGeneratorRefs.Add(goalGenerator);
	}
	
}


// Called every frame
void UGoalGeneratorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	for (UBaseGoalGenerator* GoalGenerator : M_GoalGeneratorRefs)
	{
		GoalGenerator->EvaluateGoal(GetOwner());
	}
}

void UGoalGeneratorComponent::AddGoal(UBaseGoalData* GoalData)
{
	//TODO : ensure that 2 goal datas of the same type cannot be added 
	M_CurrentGoalDatas.Add(GoalData);
}

void UGoalGeneratorComponent::RemoveGoal(UBaseGoalData* GoalData)
{
	M_CurrentGoalDatas.Remove(GoalData);
}


