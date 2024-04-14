// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseBehaviour.h"
#include "GoalGeneratorComponent.h"
#include "Components/ActorComponent.h"
#include "BehaviourSelectorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
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

public:
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<UBaseBehaviour>> M_BehaviourList;
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<UBaseBehaviour>> M_PriorityBehaviourList;
	// Sets default values for this component's properties
	UBehaviourSelectorComponent();

private:
	void ChooseBehaviourToRun();
	void SwitchToNewBehaviour();
	bool TryAndGetGoalDataClass(TSubclassOf<UBaseGoalData> goalDataClass, OUT UBaseGoalData* OutGoaldata);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
		
};
