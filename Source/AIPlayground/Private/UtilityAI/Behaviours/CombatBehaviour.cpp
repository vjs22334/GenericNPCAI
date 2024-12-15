// Fill out your copyright notice in the Description page of Project Settings.


#include "UtilityAI/Behaviours/CombatBehaviour.h"

#include "AIController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Character/ShooterBaseCharacter.h"
#include "Character/weaponInterface.h"
#include "GameFramework/Character.h"
#include "Tasks/AITask_MoveTo.h"
#include "Widgets/Text/ISlateEditableTextWidget.h"

void UCombatBehaviour::MoveToTarget()
{
	if (M_AIController)
	{
		M_CanShoot = false;
		M_AIController->K2_ClearFocus();
		
		UPathFollowingComponent* PFComponent = M_AIController->GetPathFollowingComponent();
		FAIMoveRequest MoveReq;
		MoveReq.SetAllowPartialPath(true);
		MoveReq.SetAcceptanceRadius(200);
		MoveReq.SetCanStrafe(true);
		MoveReq.SetReachTestIncludesAgentRadius(true);
		MoveReq.SetReachTestIncludesGoalRadius(true);
		MoveReq.SetUsePathfinding(true);
		MoveReq.SetGoalActor(M_TargetActor);
		
		const FPathFollowingRequestResult RequestResult = M_AIController->MoveTo(MoveReq);
		switch (RequestResult.Code)
		{
		case EPathFollowingRequestResult::Failed:
			M_BehaviourState = BehaviourExecutionState::FAILED;
			break;

		case EPathFollowingRequestResult::AlreadyAtGoal:
			OnMoveRequestFinished(RequestResult.MoveId, FPathFollowingResult(EPathFollowingResult::Success, FPathFollowingResultFlags::AlreadyAtGoal));
			break;

		case EPathFollowingRequestResult::RequestSuccessful:
			{
				PathFinishDelegateHandle = PFComponent->OnRequestFinished.AddUObject(this, &UCombatBehaviour::OnMoveRequestFinished);
				break;
			}
		default:
			break;
		}
	}
}

void UCombatBehaviour::BehaviourEnter_Implementation(UBaseGoalData* GoalData)
{
	Super::BehaviourEnter_Implementation(GoalData);
	M_GoalData = dynamic_cast<UCombatGoalData*>(GoalData);
	if (M_GoalData == nullptr || M_GoalData->Targets.Num() == 0)
	{
		M_BehaviourState = BehaviourExecutionState::FAILED;
		return;
	}
	M_TargetActor = M_GoalData->Targets[0];

	if (M_TargetActor == nullptr)
	{
		M_BehaviourState = BehaviourExecutionState::FAILED;
		return;
	}
	
	M_AIController = UAIBlueprintHelperLibrary::GetAIController(GoalOwner);

	if (M_AIController == nullptr)
	{
		M_BehaviourState = BehaviourExecutionState::FAILED;
		return;
	}

	M_BehaviourState = BehaviourExecutionState::RUNNING;
	M_BehaviourInterruptibilityState = BehaviourInterruptibilityState::INTERRUPTIBLE;

	//start a move to
	MoveToTarget();
	
}

void UCombatBehaviour::BehaviourExit_Implementation()
{
	if (M_AIController != nullptr)
	{
		M_AIController->StopMovement();
	}
	M_TargetActor = nullptr;
	M_GoalData = nullptr;
	M_AIController = nullptr;
	Super::BehaviourExit_Implementation();
}

void UCombatBehaviour::BehaviourTick_Implementation(float DeltaTime)
{
	Super::BehaviourTick_Implementation(DeltaTime);
	if (M_GoalData != nullptr && M_GoalData->Targets.Num() == 0)
	{
		//we have lost the target
		M_BehaviourState = BehaviourExecutionState::FAILED;
	}
	else
	{
		if (M_CanShoot)
		{
			if (AShooterBaseCharacter* targetCharacter = Cast<AShooterBaseCharacter>(M_GoalData->Targets[0]))
			{
				M_AIController->SetFocalPoint(targetCharacter->GetMesh()->GetSocketLocation("spine_01"));
			}
			else
			{
				M_AIController->SetFocus(M_TargetActor);
			}
			FireWeapon();
		}
	}
}

bool UCombatBehaviour::CheckPreConditions_Implementation(UBaseGoalData* GoalData)
{
	return true;
}

float UCombatBehaviour::GetSelectionScore_Implementation(UBaseGoalData* GoalData)
{
	return 10.0f;
}

void UCombatBehaviour::CleanUpPathFollowingDelegate()
{
	if (PathFinishDelegateHandle.IsValid())
	{
		if (UPathFollowingComponent* PFComp = M_AIController->GetPathFollowingComponent())
		{
			PFComp->OnRequestFinished.Remove(PathFinishDelegateHandle);
		}

		PathFinishDelegateHandle.Reset();
	}
}

void UCombatBehaviour::FireWeapon()
{
	if (ACharacter* ownerChar = UECasts_Private::DynamicCast<ACharacter*>(GoalOwner))
	{
		if (ownerChar->Implements<UWeaponInterface>())
		{
			IWeaponInterface::Execute_FireWeapon(ownerChar);
		}
	}
}

void UCombatBehaviour::OnMoveRequestFinished(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	if (Result.IsSuccess())
	{
		// M_BehaviourState = BehaviourExecutionState::COMPLETED;
		M_CanShoot = true;
	}
	else
	{
		M_BehaviourState = BehaviourExecutionState::FAILED;
	}

	CleanUpPathFollowingDelegate();
}
