// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataProviders/AIDataProvider.h"
#include "EnvironmentQuery/Generators/EnvQueryGenerator_ProjectedPoints.h"
#include "UEnvQueryGenerator_GridCustom.generated.h"

/**
 * 
 */
UCLASS()
class AIPLAYGROUND_API UUEnvQueryGenerator_GridCustom : public UEnvQueryGenerator_ProjectedPoints
{
	GENERATED_BODY()

	/** half of square's extent, like a radius */
	UPROPERTY(EditDefaultsOnly, Category=Generator, meta=(DisplayName="GridHalfSizeX"))
	FAIDataProviderFloatValue GridSizeX;

	UPROPERTY(EditDefaultsOnly, Category=Generator, meta=(DisplayName="GridHalfSizeY"))
	FAIDataProviderFloatValue GridSizeY;

	/** generation density */
	UPROPERTY(EditDefaultsOnly, Category=Generator)
	FAIDataProviderFloatValue SpaceBetween;

	/** context */
	UPROPERTY(EditDefaultsOnly, Category=Generator)
	TSubclassOf<UEnvQueryContext> GenerateAround;

	virtual void GenerateItems(FEnvQueryInstance& QueryInstance) const override;

	virtual FText GetDescriptionTitle() const override;
	virtual FText GetDescriptionDetails() const override;
};
