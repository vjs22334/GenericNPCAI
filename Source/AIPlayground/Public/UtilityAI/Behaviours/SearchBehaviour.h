// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BaseBehaviour.h"
#include "UtilityAI/GoalGenerators/SearchGoalData.h"
#include "SearchBehaviour.generated.h"

/**
 * 
 */
UCLASS()
class AIPLAYGROUND_API USearchBehaviour : public UBaseBehaviour
{
	GENERATED_BODY()

public:
	virtual void BehaviourEnter_Implementation(UBaseGoalData* GoalData) override;
	virtual void BehaviourExit_Implementation() override;
	virtual void BehaviourTick_Implementation(float DeltaTime) override;
	virtual bool CheckPreConditions_Implementation() override;
	virtual float GetSelectionScore_Implementation(UBaseGoalData* GoalData) override;
	virtual void Initialize_Implementation(AActor* OwnerActor, UBehaviourSelectorComponent* owner) override;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Utility AI")
	float SearchRadius = 500.0f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Utility AI")
	float SearchTime = 10.0f;

private:

	UPROPERTY()
	USearchGoalData* M_SearchGoalData = nullptr;
	FVector SearchLocation;
	float M_TimeElapsedSinceBehaviourStart;
	UPROPERTY()
	AAIController* M_AIController;
	FDelegateHandle PathFinishDelegateHandle;

	void MoveToRandomPosition();
	void CleanUpPathFinishDelegate();
	virtual void OnMoveRequestFinished(FAIRequestID RequestID, const FPathFollowingResult& Result);

};
