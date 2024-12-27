// Fill out your copyright notice in the Description page of Project Settings.


#include "RepositionBehaviour.h"

#include "AIController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Character/ShooterBaseCharacter.h"

void URepositionBehaviour::BehaviourEnter_Implementation(UBaseGoalData* GoalData)
{
	Super::BehaviourEnter_Implementation(GoalData);
	M_GoalData = dynamic_cast<UCombatGoalData*>(GoalData);

	if (M_GoalData == nullptr || (M_GoalData->SelfTargets.Num() == 0 && M_GoalData->TeamTargets.Num() == 0))
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
	
	
}

void URepositionBehaviour::BehaviourExit_Implementation()
{
	Super::BehaviourExit_Implementation();
	if (M_AIController != nullptr)
	{
		M_AIController->StopMovement();
		M_AIController->K2_ClearFocus();
	}
	M_GoalData = nullptr;
	M_AIController = nullptr;
	Super::BehaviourExit_Implementation();
}

void URepositionBehaviour::BehaviourTick_Implementation(float DeltaTime)
{
	Super::BehaviourTick_Implementation(DeltaTime);
	if (M_GoalData == nullptr || ( M_GoalData->SelfTargets.Num() == 0 && M_GoalData->TeamTargets.Num() == 0))
	{
		//we have no visible targets
		M_BehaviourState = BehaviourExecutionState::FAILED;
	}
	else
	{
		RefreshTargetLists(M_GoalData);
		if (M_SelectedTargetActor != nullptr)
		{
			if (!M_AllVisibleTargets.Contains(M_SelectedTargetActor))
			{
				M_SelectedTargetActor = nullptr;
			}
		}
		
		if (M_SelectedTargetActor == nullptr)
		{
			if (M_IsMoving)
			{
				M_AIController->StopMovement();
			}
			// select a new target
			int i = FMath::RandRange(0,M_AllVisibleTargets.Num()-1);
			M_SelectedTargetActor = M_AllVisibleTargets[i];
		}

		if (!M_IsMoving && M_BehaviourState == BehaviourExecutionState::RUNNING)
		{
			MoveToFiringPosition();
		}
	}
}

bool URepositionBehaviour::CheckPreConditions_Implementation(UBaseGoalData* GoalData)
{
	//do I have ammo in clip
	if (!IsAmmoInClip())
	{
		return false;
	}
	//DO we have a target to go to?
	if (UCombatGoalData* CombatGoalData = Cast<UCombatGoalData>(GoalData))
	{
		RefreshTargetLists(CombatGoalData);
		return M_AllVisibleTargets.Num() > 0;
	}
	
	return false;
}

bool URepositionBehaviour::IsAmmoInClip()
{
	if (M_GoalOwner->Implements<UWeaponInterface>())
	{
		if (IWeaponInterface::Execute_GetAmmoInClip(GoalOwner) > 0)
		{
			return true;
		}
	}
	
	return false;
}

float URepositionBehaviour::GetSelectionScore_Implementation(UBaseGoalData* GoalData)
{
	return 10;
}

void URepositionBehaviour::Initialize_Implementation(AActor* OwnerActor, UBehaviourSelectorComponent* owner)
{
	Super::Initialize_Implementation(OwnerActor, owner);
	M_GoalOwner = Cast<AShooterBaseCharacter>(GoalOwner);
	if (M_GoalOwner->Implements<UWeaponInterface>())
	{
		UWeaponProperties* weapondata = IWeaponInterface::Execute_GetWeaponProperties(M_GoalOwner);
		M_WeaponRange = weapondata->WeaponBulletRange;
	}
}

void URepositionBehaviour::OnMoveRequestFinished(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	//Do we need to do anything on success or failure here?
	if (Result.IsSuccess())
	{
	}
	else
	{
	}
	M_IsMoving = false;
	CleanUpPathFollowingDelegate();
	M_BehaviourState = BehaviourExecutionState::COMPLETED;
}

void URepositionBehaviour::CleanUpPathFollowingDelegate()
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

void URepositionBehaviour::MoveToFiringPosition()
{
	int AcceptanceRadius = 100;

	if (M_AIController)
	{
		M_AIController->K2_ClearFocus();
		
		UPathFollowingComponent* PFComponent = M_AIController->GetPathFollowingComponent();
		FAIMoveRequest MoveReq;
		MoveReq.SetAllowPartialPath(true);
		MoveReq.SetAcceptanceRadius(AcceptanceRadius);
		MoveReq.SetCanStrafe(true);
		MoveReq.SetReachTestIncludesAgentRadius(true);
		MoveReq.SetReachTestIncludesGoalRadius(true);
		MoveReq.SetUsePathfinding(true);
		MoveReq.SetGoalActor(M_SelectedTargetActor);
		
		const FPathFollowingRequestResult RequestResult = M_AIController->MoveTo(MoveReq);
		switch (RequestResult.Code)
		{
		case EPathFollowingRequestResult::Failed:
			M_SelectedTargetActor = nullptr;
			break;

		case EPathFollowingRequestResult::AlreadyAtGoal:
			OnMoveRequestFinished(RequestResult.MoveId, FPathFollowingResult(EPathFollowingResult::Success, FPathFollowingResultFlags::AlreadyAtGoal));
			break;

		case EPathFollowingRequestResult::RequestSuccessful:
			{
				PathFinishDelegateHandle = PFComponent->OnRequestFinished.AddUObject(this, &URepositionBehaviour::OnMoveRequestFinished);
				M_IsMoving = true;
				break;
			}
		default:
			break;
		}
	}
}

void URepositionBehaviour::RefreshTargetLists(UCombatGoalData* GoalData)
{
	if (GoalData == nullptr)
	{
		return;
	}
	M_AllVisibleTargets.Empty();
	M_AllVisibleTargets.Append(GoalData->SelfTargets);
	M_AllVisibleTargets.Append(GoalData->TeamTargets);
}


