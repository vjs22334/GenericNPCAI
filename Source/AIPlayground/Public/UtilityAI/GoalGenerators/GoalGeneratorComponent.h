// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseGoalGenerator.h"
#include "Components/ActorComponent.h"
#include "GoalGeneratorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AIPLAYGROUND_API UGoalGeneratorComponent : public UActorComponent
{
	GENERATED_BODY()
private:
	UPROPERTY()
	TArray<UBaseGoalGenerator*> M_GoalGeneratorRefs;
	UPROPERTY()
	TArray<UBaseGoalData*> M_CurrentGoalDatas;

public:
	const TArray<UBaseGoalData*>& GetCurrentGoalDatas() const
	{
		return M_CurrentGoalDatas;
	}

	const TArray<TSubclassOf<UBaseGoalGenerator>>& GetGoalGenerators() const
	{
		return M_GoalGenerators;
	}

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<UBaseGoalGenerator>> M_GoalGenerators;
	
public:	
	// Sets default values for this component's properties
	UGoalGeneratorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION(BlueprintCallable, Category = "UtitilyAI")
	void AddGoal(UBaseGoalData* GoalData);
	UFUNCTION(BlueprintCallable, Category = "UtitilyAI")
	void RemoveGoal(UBaseGoalData* GoalData);
};
