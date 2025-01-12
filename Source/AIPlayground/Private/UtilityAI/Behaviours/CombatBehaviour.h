// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "UtilityAI/Behaviours/BaseBehaviour.h"
#include "UtilityAI/GoalGenerators/CombatGoalData.h"
#include "CombatBehaviour.generated.h"

/**
 * 
 */
UCLASS()
class AIPLAYGROUND_API UCombatBehaviour : public UBaseBehaviour
{
	GENERATED_BODY()

public:
	void MoveToTarget();
	virtual void BehaviourEnter_Implementation(UBaseGoalData* GoalData) override;
	virtual void BehaviourExit_Implementation() override;
	virtual void BehaviourTick_Implementation(float DeltaTime) override;
	virtual bool CheckPreConditions_Implementation(UBaseGoalData* GoalData) override;
	virtual float GetSelectionScore_Implementation(UBaseGoalData* GoalData) override;
	void CleanUpPathFollowingDelegate();
	void FireWeapon();

private:
	virtual void OnMoveRequestFinished(FAIRequestID RequestID, const FPathFollowingResult& Result);

	UPROPERTY()
	AActor* M_TargetActor = nullptr;
	UPROPERTY()
	UCombatGoalData* M_GoalData = nullptr;
	FDelegateHandle PathFinishDelegateHandle;

	bool M_CanShoot = false;
	UPROPERTY()
	AAIController* M_AIController;
};
