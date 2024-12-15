// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseGoalGenerator.h"
#include "CombatGoalData.h"
#include "SearchGoalData.h"
#include "Global/DetectionSystem.h"
#include "Perception/AIPerceptionTypes.h"
#include "CombatGoalGenerator.generated.h"

/**
 * 
 */
UCLASS()
class AIPLAYGROUND_API UCombatGoalGenerator : public UBaseGoalGenerator
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void SetupPerception(AActor* goalOwner);
	virtual void EvaluateGoal_Implementation(AActor* goalOwner, float DeltaTime) override;
	virtual void Initialize_Implementation(AActor* goalOwner, UGoalGeneratorComponent* owner) override;

private:
	UPROPERTY()
	AActor* M_GoalOwner = nullptr;
	UPROPERTY()
	AActor* M_CombatTargetActor = nullptr;
	UPROPERTY()
	UCombatGoalData* M_CombatGoalData = nullptr;
	UPROPERTY()
	USearchGoalData* M_SearchGoalData = nullptr;
	UPROPERTY()
	UGoalGeneratorComponent* M_GoalGeneratorComponent;
	UPROPERTY()
	UDetectionComponent* M_DetectionComponent;
	UPROPERTY(EditAnywhere)
	float M_SearhGoalExpiryTime = 50.0f;
	float M_TimeElapsedSinceSearchStart = 0.0f;
	bool M_PerceptionSetupSuccessful = false;
	
}; 
