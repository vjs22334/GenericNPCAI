// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CoverGoalData.h"
#include "UtilityAI/GoalGenerators/BaseGoalGenerator.h"
#include <AIModule\Classes\EnvironmentQuery\EnvQueryTypes.h>
#include "DefenseGoalGenerator.generated.h"


/**
 * 
 */
UCLASS()
class AIPLAYGROUND_API UDefenseGoalGenerator : public UBaseGoalGenerator
{
	GENERATED_BODY()

public:
	void HandleQueryResult(TSharedPtr<FEnvQueryResult> EnvQueryResult);
	UFUNCTION()
	virtual void EvaluateGoal_Implementation(AActor* goalOwner, float DeltaTime) override;
	virtual void Initialize_Implementation(AActor* goalOwner, UGoalGeneratorComponent* owner) override;
	UPROPERTY(EditAnywhere, Category = "AI")
	UEnvQuery *FindHidingSpotEQS;

private:
	UPROPERTY()
	AActor* M_GoalOwner = nullptr;
	UPROPERTY()
	UGoalGeneratorComponent* M_GoalGeneratorComponent = nullptr;
	UPROPERTY()
	UCoverGoalData* M_CoverGoalData = nullptr;
	UPROPERTY()
	FVector M_CoverLocation = FVector::Zero();

	bool M_ExecutingQuery = false;
};

