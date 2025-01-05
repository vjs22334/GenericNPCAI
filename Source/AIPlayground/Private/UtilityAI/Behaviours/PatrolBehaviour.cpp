// Fill out your copyright notice in the Description page of Project Settings.


#include "UtilityAI/Behaviours/PatrolBehaviour.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "SAdvancedTransformInputBox.h"
#include "Global/DetectionSystem.h"
#include "UtilityAI/GoalGenerators/SearchGoalData.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Navigation/PathFollowingComponent.h"

void UPatrolBehaviour::BehaviourEnter_Implementation(UBaseGoalData* GoalData)
{
	Super::BehaviourEnter_Implementation(GoalData);

	M_BehaviourInterruptibilityState = BehaviourInterruptibilityState::INTERRUPTIBLE;
	M_BehaviourState = BehaviourExecutionState::RUNNING;
	
	M_AIController = UAIBlueprintHelperLibrary::GetAIController(GoalOwner);
	M_SearchGoalData = Cast<USearchGoalData>(GoalData);
	M_LastKnownLocation = M_SearchGoalData->MoveToLocation;

	if (M_AIController != nullptr)
	{
		if (UNavigationSystemV1* const NavigationSystem = UNavigationSystemV1::GetCurrent(GetWorld()))
		{
			UPathFollowingComponent* PFComponent = M_AIController->GetPathFollowingComponent();
			FAIMoveRequest MoveReq;
			MoveReq.SetAllowPartialPath(true);
			MoveReq.SetAcceptanceRadius(50);
			MoveReq.SetCanStrafe(true);
			MoveReq.SetReachTestIncludesAgentRadius(true);
			MoveReq.SetReachTestIncludesGoalRadius(true);
			MoveReq.SetUsePathfinding(true);
			MoveReq.SetGoalLocation(M_LastKnownLocation);

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
					PathFinishDelegateHandle = PFComponent->OnRequestFinished.AddUObject(this, &UPatrolBehaviour::OnMoveRequestFinished);
				}
			}
		}
	}
}

	void UPatrolBehaviour::OnMoveRequestFinished(FAIRequestID RequestID, const FPathFollowingResult& Result)
	{
		if (Result.IsSuccess())
		{
			M_BehaviourState = BehaviourExecutionState::COMPLETED;
		}
		else
		{
			M_BehaviourState = BehaviourExecutionState::FAILED;
		}
		CleanUpPathFollowingDelegate();
	}

	void UPatrolBehaviour::CleanUpPathFollowingDelegate()
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

void UPatrolBehaviour::BehaviourExit_Implementation()
{
	Super::BehaviourExit_Implementation();
	
	if (M_AIController != nullptr)
	{
		M_AIController->StopMovement();
	}
}

void UPatrolBehaviour::BehaviourTick_Implementation(float DeltaTime)
{
	DrawDebugSphere(GetWorld(),M_LastKnownLocation,200 ,16,FColor(255, 0, 0),false, -1, 0,5);
}

bool UPatrolBehaviour::CheckPreConditions_Implementation(UBaseGoalData* GoalData)
{
	USearchGoalData* SearchGoalData = Cast<USearchGoalData>(GoalData);
	if (SearchGoalData != nullptr)
	{
		if (SearchGoalData->MoveToLocation != FVector::ZeroVector)
		{
			return true;
		}
	}
	return false;
}

float UPatrolBehaviour::GetSelectionScore_Implementation(UBaseGoalData* GoalData)
{
	return 5;
}

void UPatrolBehaviour::Initialize_Implementation(AActor* OwnerActor, UBehaviourSelectorComponent* owner)
{
	Super::Initialize_Implementation(OwnerActor, owner);
}


