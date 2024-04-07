// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviourSelectorComponent.h"
#include "UObject/NoExportTypes.h"
#include "UtilityAI/BaseGoalData.h"
#include "BaseBehaviour.generated.h"

/**
 * 
 */
UCLASS()
class UBaseBehaviour : public UObject
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	TSubclassOf<UBaseGoalData> GoalDataClass;
	
public:
	bool CheckPreConditions();
	void Initialize(AActor* OwnerActor, UBehaviourSelectorComponent* owner);
	void BehaviourEnter(UBaseGoalData* GoalData);
	void BehavourTick(UBaseGoalData* GoalData);
	float GetSelectionScore();
	
};
