// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UtilityAI/Behaviours/BaseBehaviour.h"
#include "Global/DetectionSystem.h"
#include "UtilityAI/GoalGenerators/SearchGoalData.h"
#include "AIController.h"
#include "PatrolBehaviour.generated.h"


/**
 * 
 */
UCLASS()
class AIPLAYGROUND_API UPatrolBehaviour : public UBaseBehaviour
{
	GENERATED_BODY()

public:
	virtual void BehaviourEnter_Implementation(UBaseGoalData* GoalData) override;
	virtual void BehaviourExit_Implementation() override;
	virtual void BehaviourTick_Implementation(float DeltaTime) override;
	virtual bool CheckPreConditions_Implementation(UBaseGoalData* GoalData) override;
	virtual float GetSelectionScore_Implementation(UBaseGoalData* GoalData) override;
	virtual void Initialize_Implementation(AActor* OwnerActor, UBehaviourSelectorComponent* owner) override;

private:
	FVector M_LastKnownLocation;
	UPROPERTY()
	USearchGoalData* M_SearchGoalData = nullptr;
	AAIController* M_AIController;

	void OnMoveRequestFinished(FAIRequestID RequestID, const FPathFollowingResult& Result);
	void CleanUpPathFollowingDelegate();
	FDelegateHandle PathFinishDelegateHandle;

};
