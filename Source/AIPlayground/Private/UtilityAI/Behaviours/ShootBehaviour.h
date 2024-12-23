// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AITypes.h"
#include "Character/ShooterBaseCharacter.h"
#include "Navigation/PathFollowingComponent.h"
#include "UtilityAI/Behaviours/BaseBehaviour.h"
#include "UtilityAI/GoalGenerators/CombatGoalData.h"
#include "ShootBehaviour.generated.h"

/**
 * 
 */
UCLASS()
class AIPLAYGROUND_API UShootBehaviour : public UBaseBehaviour
{
	GENERATED_BODY()

public:
	virtual void BehaviourEnter_Implementation(UBaseGoalData* GoalData) override;
	virtual void BehaviourExit_Implementation() override;
	void ShootTarget();
	virtual void BehaviourTick_Implementation(float DeltaTime) override;
	bool IsAmmoInClip();
	virtual bool CheckPreConditions_Implementation(UBaseGoalData* GoalData) override;
	virtual float GetSelectionScore_Implementation(UBaseGoalData* GoalData) override;
	virtual void Initialize_Implementation(AActor* OwnerActor, UBehaviourSelectorComponent* owner) override;
	void CleanUpPathFollowingDelegate();
	virtual void OnMoveRequestFinished(FAIRequestID RequestID, const FPathFollowingResult& Result);
	void MoveToFiringPosition();
	void FireWeapon();

private:
	bool ChecKWeaponLos(AActor* TargetActor);
	bool CheckWithinWeaponRange(AActor* TargetActor);

	
	UPROPERTY()
	AActor* M_SelectedTargetActor = nullptr;
	UPROPERTY()
	UCombatGoalData* M_GoalData = nullptr;
	FDelegateHandle PathFinishDelegateHandle;

	float M_WeaponRange = 100.0f;

	bool M_CanShoot = false;
	bool M_IsMovingToFiringPosition = false;
	UPROPERTY()
	AAIController* M_AIController;
	UPROPERTY()
	AShooterBaseCharacter* M_GoalOwner;


};
