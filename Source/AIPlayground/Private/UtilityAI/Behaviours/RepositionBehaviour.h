// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Character/ShooterBaseCharacter.h"
#include "Character/WeaponProperties.h"
#include "UtilityAI/Behaviours/BaseBehaviour.h"
#include "UtilityAI/GoalGenerators/CombatGoalData.h"
#include "RepositionBehaviour.generated.h"

/**
 * 
 */
UCLASS()
class AIPLAYGROUND_API URepositionBehaviour : public UBaseBehaviour
{
	GENERATED_BODY()

public:
	virtual void BehaviourEnter_Implementation(UBaseGoalData* GoalData) override;
	virtual void BehaviourExit_Implementation() override;
	virtual void BehaviourTick_Implementation(float DeltaTime) override;
	virtual bool CheckPreConditions_Implementation(UBaseGoalData* GoalData) override;
	virtual float GetSelectionScore_Implementation(UBaseGoalData* GoalData) override;
	virtual void Initialize_Implementation(AActor* OwnerActor, UBehaviourSelectorComponent* owner) override;

private:
	virtual void OnMoveRequestFinished(FAIRequestID RequestID, const FPathFollowingResult& Result);
	void CleanUpPathFollowingDelegate();
	void MoveToFiringPosition();
	void RefreshTargetLists(UCombatGoalData* GoalData);
	bool IsAmmoInClip();

	UPROPERTY()
	AActor* M_SelectedTargetActor = nullptr;
	UPROPERTY()
	AShooterBaseCharacter* M_GoalOwner;
	UPROPERTY()
	AAIController* M_AIController;
	UPROPERTY()
	TArray<AActor*> M_AllVisibleTargets;
	float M_WeaponRange = 100;
	UPROPERTY()
	UCombatGoalData* M_GoalData;
	FDelegateHandle PathFinishDelegateHandle;
	bool M_IsMoving = false;

};

