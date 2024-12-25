// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "UtilityAI/Behaviours/BaseBehaviour.h"
#include "UtilityAI/GoalGenerators/CoverGoalData.h"
#include "CoverBehaviour.generated.h"

/**
 * 
 */
UCLASS()
class AIPLAYGROUND_API UCoverBehaviour : public UBaseBehaviour
{
	GENERATED_BODY()
public:
	virtual void BehaviourEnter_Implementation(UBaseGoalData* GoalData) override;
	virtual void BehaviourExit_Implementation() override;
	virtual void BehaviourTick_Implementation(float DeltaTime) override;
	virtual bool CheckPreConditions_Implementation(UBaseGoalData* GoalData) override;
	virtual float GetSelectionScore_Implementation(UBaseGoalData* GoalData) override;
	virtual void Initialize_Implementation(AActor* OwnerActor, UBehaviourSelectorComponent* owner) override;
	UPROPERTY(EditAnywhere)
	float MaxEQSQueryTime = 5;
	UPROPERTY(EditAnywhere)
	float MaxCrouchTime = 5;
	
	

private:
	UPROPERTY()
	UCoverGoalData* M_CoverGoalData = nullptr;
	FVector M_CurrentCoverLocation;
	AAIController* M_AIController;
	float M_TimeElapsedSinceEQSQueryStart = 0;
	float M_TimeElapsedSinceCrouchStart = 0;
	FDelegateHandle PathFinishDelegateHandle;
	void OnMoveRequestFinished(FAIRequestID RequestID, const FPathFollowingResult& Result);;
	bool isMoving = false;
	enum CoverBehaviourState
	{
		WaitingForEQS,
		Moving,
		Crouching
	};
	CoverBehaviourState CurrentState = WaitingForEQS;
};

