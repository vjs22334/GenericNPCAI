// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UtilityAI/BaseGoalData.h"
#include "BaseBehaviour.generated.h"

class UBehaviourSelectorComponent;

UENUM(BlueprintType)
enum class BehaviourExecutionState : uint8
{
	RUNNING = 0,
	COMPLETED,
	FAILED,
	NOTRUNNING
};

UENUM(BlueprintType)
enum class BehaviourInterruptibilityState : uint8
{
	INTERRUPTIBLE = 0,
	NOTINTERRUPTIBLE
};

/**
 * 
 */
UCLASS()
class UBaseBehaviour : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	BehaviourExecutionState M_BehaviourState;
	UPROPERTY()
	BehaviourInterruptibilityState M_BehaviourInterruptibilityState;

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UBaseGoalData> GoalDataClass;
	
public:
	bool CheckPreConditions();
	void Initialize(AActor* OwnerActor, UBehaviourSelectorComponent* owner);
	void BehaviourEnter(UBaseGoalData* GoalData);
	void BehaviourTick(UBaseGoalData* GoalData);
	void BehaviourExit(UBaseGoalData* GoalData);
	float GetSelectionScore(UBaseGoalData* GoalData);
	BehaviourExecutionState GetBehaviourExecutionState();
	BehaviourInterruptibilityState GetBehaviourInterruptibiliyState();
	
};
