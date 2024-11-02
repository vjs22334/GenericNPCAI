// Fill out your copyright notice in the Description page of Project Settings.


#include "UtilityAI/Behaviours/CombatBehaviour.h"

#include "AIController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Tasks/AITask_MoveTo.h"

void UCombatBehaviour::MoveToTarget()
{
	M_AIController = UAIBlueprintHelperLibrary::GetAIController(GoalOwner);
	if (M_AIController)
	{
		UPathFollowingComponent* PFComponent = M_AIController->GetPathFollowingComponent();
		FAIMoveRequest MoveReq;
		MoveReq.SetAllowPartialPath(true);
		MoveReq.SetAcceptanceRadius(50);
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
	M_GoalData = reinterpret_cast<UCombatGoalData*>(GoalData);
	if (M_GoalData == nullptr)
	{
		M_BehaviourState = BehaviourExecutionState::FAILED;
		return;
	}
	M_TargetActor = M_GoalData->TargetActor;

	if (M_TargetActor == nullptr)
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
	Super::BehaviourExit_Implementation();
}

void UCombatBehaviour::BehaviourTick_Implementation()
{
	Super::BehaviourTick_Implementation();
}

bool UCombatBehaviour::CheckPreConditions_Implementation()
{
	return Super::CheckPreConditions_Implementation();
}

float UCombatBehaviour::GetSelectionScore_Implementation(UBaseGoalData* GoalData)
{
	return 10.0f;
}

void UCombatBehaviour::OnMoveRequestFinished(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	if (Result.IsSuccess())
	{
		M_BehaviourState = BehaviourExecutionState::COMPLETED;
	}
	else
	{
		M_BehaviourState = BehaviourExecutionState::FAILED;
	}

	if (PathFinishDelegateHandle.IsValid())
	{
		if (UPathFollowingComponent* PFComp = M_AIController->GetPathFollowingComponent())
		{
			PFComp->OnRequestFinished.Remove(PathFinishDelegateHandle);
		}

		PathFinishDelegateHandle.Reset();
	}
}
