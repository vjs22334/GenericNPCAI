// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "UtilityAI/Behaviours/BaseBehaviour.h"
#include "UtilityAI/GoalGenerators/CoverGoalData.h"
#include <AIPlayground/Private/Character/ShooterBaseCharacter.h>
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
	void SelectCoverAndStartMove();
	virtual void BehaviourExit_Implementation() override;
	virtual void BehaviourTick_Implementation(float DeltaTime) override;
	virtual bool CheckPreConditions_Implementation(UBaseGoalData* GoalData) override;
	virtual float GetSelectionScore_Implementation(UBaseGoalData* GoalData) override;
	virtual void Initialize_Implementation(AActor* OwnerActor, UBehaviourSelectorComponent* owner) override;
	
	

	UPROPERTY(EditAnywhere)
	float MaxCrouchTime = 5;
	
	

private:
	void OnMoveRequestFinished(FAIRequestID RequestID, const FPathFollowingResult& Result);
	void CleanUpPathFollowingDelegate();


	
	UPROPERTY()
	UCoverGoalData* M_CoverGoalData = nullptr;
	UPROPERTY()
	AShooterBaseCharacter* M_ShooterBaseGoalOwner = nullptr;
	FVector M_CurrentCoverLocation;
	AAIController* M_AIController;
	float M_TimeElapsedSinceEQSQueryStart = 0;
	float M_TimeElapsedSinceCrouchStart = 0;
	FDelegateHandle PathFinishDelegateHandle;
	bool isMoving = false;
	enum CoverBehaviourState
	{
		Moving,
		Crouching
	};
	CoverBehaviourState CurrentState = Moving;

	bool M_isReloading = false;
};

