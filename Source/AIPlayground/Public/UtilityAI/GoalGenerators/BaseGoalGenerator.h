// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseGoalData.h"
#include "UObject/NoExportTypes.h"
#include "BaseGoalGenerator.generated.h"

class UGoalGeneratorComponent;
/**
 * 
 */
UCLASS(Blueprintable)
class AIPLAYGROUND_API UBaseGoalGenerator : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent,Category="UtitilyAI")
	void EvaluateGoal(AActor* goalOwner);

	UFUNCTION(BlueprintNativeEvent,Category="UtitilyAI")
	void Initialize(AActor* goalOwner, UGoalGeneratorComponent* owner);
};
