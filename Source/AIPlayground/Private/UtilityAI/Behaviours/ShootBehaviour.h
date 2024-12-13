// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UtilityAI/Behaviours/BaseBehaviour.h"
#include "ShootBehaviour.generated.h"

/**
 * 
 */
UCLASS()
class AIPLAYGROUND_API UShootBehaviour : public UBaseBehaviour
{
	GENERATED_BODY()

public:
	virtual void BehaviourEnter_Implementation(UBaseGoalData* GoalData) override;
	virtual void BehaviourExit_Implementation() override;
	virtual void BehaviourTick_Implementation(float DeltaTime) override;
	virtual bool CheckPreConditions_Implementation(UBaseGoalData* GoalData) override;
	virtual float GetSelectionScore_Implementation(UBaseGoalData* GoalData) override;
	virtual void Initialize_Implementation(AActor* OwnerActor, UBehaviourSelectorComponent* owner) override;
	
};
