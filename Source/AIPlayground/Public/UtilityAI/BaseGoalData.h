// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BaseGoalData.generated.h"

/**
 * 
 */
UCLASS()
class AIPLAYGROUND_API UBaseGoalData : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
	bool IsDirty;
};
