// Fill out your copyright notice in the Description page of Project Settings.


#include "UtilityAI\Behaviours/CoverBehaviour.h"

#include "AITypes.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NavigationSystem.h"
#include "Character/ShooterBaseCharacter.h"
#include "GameFramework/Character.h"
#include "Global/EQSReservationSystem.h"
#include "Navigation/PathFollowingComponent.h"


void UCoverBehaviour::BehaviourEnter_Implementation(UBaseGoalData* GoalData)
{
	Super::BehaviourEnter_Implementation(GoalData);

	M_BehaviourInterruptibilityState = BehaviourInterruptibilityState::NOTINTERRUPTIBLE;
	
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
	isMoving = false;
	CurrentState = Moving;
	SelectCoverAndStartMove();
	
}

void UCoverBehaviour::SelectCoverAndStartMove()
{
	M_CurrentCoverLocation = M_CoverGoalData-> CoverLocation;
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
					if (UEQSReservationSystem* CoverReservationSystem = GetWorld()->GetGameInstance()->GetSubsystem<UEQSReservationSystem>())
					{
						CoverReservationSystem->ReserveCoverLocation(M_CurrentCoverLocation);
					}
					isMoving = true;
					break;
				}
			default:
				break;
			}
		}
	}
}

void UCoverBehaviour::BehaviourTick_Implementation(float DeltaTime)
{
	Super::BehaviourTick_Implementation(DeltaTime);

	switch (CurrentState)
	{
	case Moving :
		DrawDebugSphere(GetWorld(),M_CurrentCoverLocation,200 ,16,FColor(255, 0, 0),false, -1, 0,5);
		if (!isMoving)
		{
			SelectCoverAndStartMove();
		}
		break;
	case Crouching :
		M_ShooterBaseGoalOwner->InCover = true;
		if (M_TimeElapsedSinceCrouchStart > MaxCrouchTime)
		{
			M_BehaviourInterruptibilityState = BehaviourInterruptibilityState::INTERRUPTIBLE;
			M_BehaviourState = BehaviourExecutionState::COMPLETED;
		}
		if (IWeaponInterface::Execute_GetPercentAmmoLeftInClip(M_ShooterBaseGoalOwner) < 100 && !M_isReloading)
		{
			M_isReloading = true;
			IWeaponInterface::Execute_ReloadWeapon(M_ShooterBaseGoalOwner);
		}
		M_TimeElapsedSinceCrouchStart += DeltaTime;
		break;
	}
}

bool UCoverBehaviour::CheckPreConditions_Implementation(UBaseGoalData* GoalData)
{
	UCoverGoalData* CoverGoalData = Cast<UCoverGoalData>(GoalData);
	if (CoverGoalData != nullptr && CoverGoalData->CoverLocation != FVector::ZeroVector)
	{
		return true;
	}
	
	return false;
	
}

float UCoverBehaviour::GetSelectionScore_Implementation(UBaseGoalData* GoalData)
{
	if (AShooterBaseCharacter* shooterBaseCharacter = dynamic_cast<AShooterBaseCharacter*>(GoalOwner))
	{
		if (IHealthSystemInterface::Execute_GetIsTakingDamage(shooterBaseCharacter))
		{
			return 30;
		}
		
		if (IWeaponInterface::Execute_GetPercentAmmoLeftInClip(shooterBaseCharacter) < 30 )
		{
			return 15;
		}
	}
	return 4;
}

void UCoverBehaviour::Initialize_Implementation(AActor* OwnerActor, UBehaviourSelectorComponent* owner)
{
	Super::Initialize_Implementation(OwnerActor, owner);
	M_ShooterBaseGoalOwner = Cast<AShooterBaseCharacter>(OwnerActor);
}

void UCoverBehaviour::OnMoveRequestFinished(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	if (Result.IsSuccess())
	{
		ACharacter* OwnerActor = Cast<ACharacter>(GoalOwner);
		if (OwnerActor != nullptr)
		{
			CurrentState = Crouching;
			M_TimeElapsedSinceCrouchStart = 0;
			OwnerActor->Crouch();
		}
	}
	else
	{
		M_BehaviourState = BehaviourExecutionState::FAILED;
	}
	isMoving = false;
	CleanUpPathFollowingDelegate();
}

void UCoverBehaviour::CleanUpPathFollowingDelegate()
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

void UCoverBehaviour::BehaviourExit_Implementation()
{
	if (UEQSReservationSystem* CoverReservationSystem = GetWorld()->GetGameInstance()->GetSubsystem<UEQSReservationSystem>())
	{
		CoverReservationSystem->UnReserveCoverLocation(M_CurrentCoverLocation);
	}
	if (M_AIController != nullptr)
	{
		M_AIController->StopMovement();
	}
	if (M_ShooterBaseGoalOwner != nullptr)
	{
		M_ShooterBaseGoalOwner->UnCrouch();
		M_ShooterBaseGoalOwner->InCover = false;
	}
}

	






