// Fill out your copyright notice in the Description page of Project Settings.


#include "UtilityAI/Behaviours/SearchBehaviour.h"

#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "UtilityAI/Behaviours/CombatBehaviour.h"

void USearchBehaviour::BehaviourEnter_Implementation(UBaseGoalData* GoalData)
{
	Super::BehaviourEnter_Implementation(GoalData);

	M_BehaviourInterruptibilityState = BehaviourInterruptibilityState::INTERRUPTIBLE;
	
	M_BehaviourState = BehaviourExecutionState::RUNNING;

	M_SearchGoalData = dynamic_cast<USearchGoalData*>(GoalData);
	if (M_SearchGoalData == nullptr)
	{
		M_BehaviourState = BehaviourExecutionState::FAILED;
		return;
	}

	SearchLocation = M_SearchGoalData->Location;
	M_TimeElapsedSinceBehaviourStart = 0;

	M_AIController = UAIBlueprintHelperLibrary::GetAIController(GoalOwner);
	if (M_AIController == nullptr)
	{
		M_BehaviourState = BehaviourExecutionState::FAILED;
		return;
	}
	
	MoveToRandomPosition();
	
}

void USearchBehaviour::BehaviourExit_Implementation()
{
	CleanUpPathFinishDelegate();
	if (M_AIController != nullptr)
	{
		M_AIController->StopMovement();
	}
	M_SearchGoalData = nullptr;
	M_AIController = nullptr;
	Super::BehaviourExit_Implementation();
}

void USearchBehaviour::BehaviourTick_Implementation(float DeltaTime)
{
	Super::BehaviourTick_Implementation(DeltaTime);
	M_TimeElapsedSinceBehaviourStart += DeltaTime;

	if (M_TimeElapsedSinceBehaviourStart >= SearchTime)
	{
		M_BehaviourState = BehaviourExecutionState::COMPLETED;
	}
}

bool USearchBehaviour::CheckPreConditions_Implementation()
{
	return true;
}

float USearchBehaviour::GetSelectionScore_Implementation(UBaseGoalData* GoalData)
{
	return 5.0f;
}

void USearchBehaviour::Initialize_Implementation(AActor* OwnerActor, UBehaviourSelectorComponent* owner)
{
	Super::Initialize_Implementation(OwnerActor, owner);
}

void USearchBehaviour::MoveToRandomPosition()
{
	if (M_AIController != nullptr)
	{
		if (UNavigationSystemV1* const NavigationSystem = UNavigationSystemV1::GetCurrent(GetWorld()))
		{
			FNavLocation Loc;
			if (NavigationSystem->GetRandomPointInNavigableRadius(SearchLocation, SearchRadius,Loc))
			{
				UPathFollowingComponent* PFComponent = M_AIController->GetPathFollowingComponent();
				FAIMoveRequest MoveReq;
				MoveReq.SetAllowPartialPath(true);
				MoveReq.SetAcceptanceRadius(50);
				MoveReq.SetCanStrafe(true);
				MoveReq.SetReachTestIncludesAgentRadius(true);
				MoveReq.SetReachTestIncludesGoalRadius(true);
				MoveReq.SetUsePathfinding(true);
				MoveReq.SetGoalLocation(Loc);

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
						PathFinishDelegateHandle = PFComponent->OnRequestFinished.AddUObject(this, &USearchBehaviour::OnMoveRequestFinished);
						break;
					}
				default:
					break;
				}
			}
			
		}
	}
}

void USearchBehaviour::CleanUpPathFinishDelegate()
{
	if (PathFinishDelegateHandle.IsValid() && M_AIController != nullptr)
	{
		if (UPathFollowingComponent* PFComp = M_AIController->GetPathFollowingComponent())
		{
			PFComp->OnRequestFinished.Remove(PathFinishDelegateHandle);
		}

		PathFinishDelegateHandle.Reset();
	}
}

void USearchBehaviour::OnMoveRequestFinished(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	CleanUpPathFinishDelegate();

	
	MoveToRandomPosition();

}
