// Fill out your copyright notice in the Description page of Project Settings.


#include "UtilityAI\Behaviours/CoverBehaviour.h"

#include "AITypes.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NavigationSystem.h"
#include "Character/ShooterBaseCharacter.h"
#include "GameFramework/Character.h"
#include "Navigation/PathFollowingComponent.h"


void UCoverBehaviour::BehaviourEnter_Implementation(UBaseGoalData* GoalData)
{
	Super::BehaviourEnter_Implementation(GoalData);

	M_BehaviourInterruptibilityState = BehaviourInterruptibilityState::INTERRUPTIBLE;
	
	M_BehaviourState = BehaviourExecutionState::RUNNING;

	M_CoverGoalData = Cast<UCoverGoalData>(GoalData);
	if (M_CoverGoalData == nullptr)
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
	
}

void UCoverBehaviour::BehaviourTick_Implementation(float DeltaTime)
{
	Super::BehaviourTick_Implementation(DeltaTime);
	M_CurrentCoverLocation = M_CoverGoalData-> CoverLocation;
	switch (CurrentState)
	{
	case WaitingForEQS :
		if (M_CurrentCoverLocation == FVector::ZeroVector)
		{
			if (M_TimeElapsedSinceEQSQueryStart > MaxEQSQueryTime)
			{
				M_BehaviourState = BehaviourExecutionState::FAILED;
				return;
			}
			M_TimeElapsedSinceEQSQueryStart += DeltaTime;
		}
		else
		{
			CurrentState = Moving;
		}
		break;
		
	case Moving :
		if (!isMoving)
		{
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
					MoveReq.SetGoalLocation(M_CurrentCoverLocation);

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
							PathFinishDelegateHandle = PFComponent->OnRequestFinished.AddUObject(this, &UCoverBehaviour::OnMoveRequestFinished);
							isMoving = true;
							break;
						}
					default:
						break;
					}
				}
			}
		}
		
	case Crouching :
		if (M_TimeElapsedSinceCrouchStart > MaxCrouchTime)
		{
			M_BehaviourInterruptibilityState = BehaviourInterruptibilityState::INTERRUPTIBLE;
			M_BehaviourState = BehaviourExecutionState::COMPLETED;
		}
		M_TimeElapsedSinceCrouchStart += DeltaTime;
	}
}

bool UCoverBehaviour::CheckPreConditions_Implementation(UBaseGoalData* GoalData)
{
	if (AShooterBaseCharacter* shooterBaseCharacter = dynamic_cast<AShooterBaseCharacter*>(GoalOwner))
	{
		return IHealthSystemInterface::Execute_GetIsTakingDamage(shooterBaseCharacter);
	}
	return false;
	
}

float UCoverBehaviour::GetSelectionScore_Implementation(UBaseGoalData* GoalData)
{
	return 30;
}

void UCoverBehaviour::Initialize_Implementation(AActor* OwnerActor, UBehaviourSelectorComponent* owner)
{
	Super::Initialize_Implementation(OwnerActor, owner);
}

void UCoverBehaviour::OnMoveRequestFinished(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	if (Result.IsSuccess())
	{
		isMoving = false;
		ACharacter* OwnerActor = Cast<ACharacter>(GoalOwner);
		if (OwnerActor != nullptr)
		{
			CurrentState = Crouching;
			OwnerActor->Crouch();
			M_BehaviourInterruptibilityState = BehaviourInterruptibilityState::NOTINTERRUPTIBLE;
		}
	}
		else
		{
			M_BehaviourState = BehaviourExecutionState::FAILED;
			return;
		}
}

void UCoverBehaviour::BehaviourExit_Implementation()
{
	ACharacter* OwnerActor = Cast<ACharacter>(GoalOwner);
	OwnerActor->UnCrouch();
}

	






