// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UtilityAI/GoalGenerators/BaseGoalData.h"
#include "UtilityAI/GoalGenerators/GoalGeneratorComponent.h"
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
UCLASS(Blueprintable)
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

	UPROPERTY(EditAnywhere)
	bool CanExecuteWithoutGoalData = false;

protected:
	UPROPERTY()
	AActor* GoalOwner = nullptr;
	UPROPERTY()
	UBehaviourSelectorComponent* BehaviourSelectorRef = nullptr;
	
public:
	UFUNCTION(BlueprintNativeEvent,Category="UtitilyAI")
	bool CheckPreConditions();
	UFUNCTION(BlueprintNativeEvent,Category="UtitilyAI")
	void Initialize(AActor* OwnerActor, UBehaviourSelectorComponent* owner);
	UFUNCTION(BlueprintNativeEvent,Category="UtitilyAI")
	void BehaviourEnter(UBaseGoalData* GoalData);
	UFUNCTION(BlueprintNativeEvent,Category="UtitilyAI")
	void BehaviourTick();
	UFUNCTION(BlueprintNativeEvent,Category="UtitilyAI")
	void BehaviourExit();
	UFUNCTION(BlueprintNativeEvent,Category="UtitilyAI")
	float GetSelectionScore(UBaseGoalData* GoalData);
	UFUNCTION(BlueprintCallable,Category="UtitilyAI")
	BehaviourExecutionState GetBehaviourExecutionState();
	UFUNCTION(BlueprintCallable,Category="UtitilyAI")
	BehaviourInterruptibilityState GetBehaviourInterruptibiliyState();
	
};
