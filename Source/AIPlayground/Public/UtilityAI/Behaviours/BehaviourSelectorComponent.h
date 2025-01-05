// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseBehaviour.h"
#include "Components/ActorComponent.h"
#include "UtilityAI/GoalGenerators/GoalGeneratorComponent.h"
#include "BehaviourSelectorComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UBehaviourSelectorComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY()
	UGoalGeneratorComponent* M_GoalGeneratorComponentRef;
	UPROPERTY()
	TArray<UBaseBehaviour*> M_BehaviourRefs;
	UPROPERTY()
	TArray<UBaseBehaviour*> M_PriorityBehaviourRefs;
	UPROPERTY()
	UBaseBehaviour* M_CurrentBehaviour = nullptr;
	UPROPERTY()
	UBaseBehaviour* M_NextChosenBehaviour = nullptr;
	UPROPERTY()
	UBaseGoalData* M_CurrentBehaviourGoalData = nullptr;
	UPROPERTY()
	UBaseGoalData* M_NextChosenBehaviourGoalData = nullptr;
	UPROPERTY()
	TArray<UBaseGoalData*> M_CurrentGoalDatas;

	UPROPERTY()
	TMap<UBaseBehaviour*, float> M_CurrentBehaviourCoolDownMap;

public:
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<UBaseBehaviour>> M_BehaviourList;
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<UBaseBehaviour>> M_PriorityBehaviourList;
	// Sets default values for this component's properties
	UBehaviourSelectorComponent();
	virtual void DestroyComponent(bool bPromoteChildren = false) override;

private:
	void ChooseBehaviourToRun();
	void SwitchToNewBehaviour();
	UBaseGoalData* TryAndGetGoalDataClass(TSubclassOf<UBaseGoalData> goalDataClass);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	FString GetCurrentBehaviourName();
};
