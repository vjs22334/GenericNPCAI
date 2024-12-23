// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootBehaviour.h"

#include "AIController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Character/ShooterBaseCharacter.h"
#include "Character/weaponInterface.h"
#include "UtilityAI/GoalGenerators/CombatGoalData.h"

void UShootBehaviour::BehaviourEnter_Implementation(UBaseGoalData* GoalData)
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
	if (M_GoalOwner->Implements<UWeaponInterface>())
	{
		UWeaponProperties* weapondata = IWeaponInterface::Execute_GetWeaponProperties(M_GoalOwner);
		M_WeaponRange = weapondata->WeaponBulletRange;
	}
	
}

void UShootBehaviour::BehaviourExit_Implementation()
{
	if (M_AIController != nullptr)
	{
		M_AIController->StopMovement();
	}
	M_SelectedTargetActor = nullptr;
	M_GoalData = nullptr;
	M_AIController = nullptr;
	Super::BehaviourExit_Implementation();
}

void UShootBehaviour::ShootTarget()
{
	if (AShooterBaseCharacter* targetCharacter = Cast<AShooterBaseCharacter>(M_SelectedTargetActor))
	{
		M_AIController->SetFocalPoint(targetCharacter->GetMesh()->GetSocketLocation("spine_01"));
	}
	else
	{
		M_AIController->SetFocus(M_SelectedTargetActor);
	}
	FireWeapon();
}

bool UShootBehaviour::ChecKWeaponLos(AActor* TargetActor)
{
	FHitResult OutHit;
	FVector endLocation = TargetActor->GetActorLocation();
	FVector startLocation = M_GoalOwner->GetMesh()->GetSocketLocation("spine_01");

	if (AShooterBaseCharacter* targetCharacter = Cast<AShooterBaseCharacter>(TargetActor))
	{
		 endLocation = targetCharacter->GetMesh()->GetSocketLocation("spine_01");
	}
	
	bool hit = GetWorld()->LineTraceSingleByChannel(OutHit, startLocation, endLocation,ECC_Visibility,FCollisionQueryParams(FName("LOSTrace"),false,M_GoalOwner));
	DrawDebugLine(
			GetWorld(),
			startLocation,
			endLocation,
			FColor(255, 0, 0),
			false, -1, 0,
			5
		);
	if (!hit || (IsValid(OutHit.GetActor()) && (OutHit.GetActor()->IsOwnedBy(TargetActor) || OutHit.GetActor()->IsOwnedBy(M_GoalOwner))))
	{
		return true;
	}
	return false;
}

bool UShootBehaviour::CheckWithinWeaponRange(AActor* TargetActor)
{
	FVector rangeToTarget = TargetActor->GetActorLocation() - M_GoalOwner->GetActorLocation();
	return (rangeToTarget.Length() <= M_WeaponRange - 10);
}

void UShootBehaviour::BehaviourTick_Implementation(float DeltaTime)
{
	Super::BehaviourTick_Implementation(DeltaTime);
	if (M_GoalData == nullptr || ( M_GoalData->SelfTargets.Num() == 0 && M_GoalData->TeamTargets.Num() == 0))
	{
		//we have no targets in line of sight
		M_BehaviourState = BehaviourExecutionState::FAILED;
	}
	else if (!IsAmmoInClip())
	{
		//we are out of ammo
		M_BehaviourState = BehaviourExecutionState::COMPLETED;
	}
	else
	{
		if (M_SelectedTargetActor != nullptr)
		{
			if ((!M_GoalData->SelfTargets.Contains(M_SelectedTargetActor))&&(!M_GoalData->TeamTargets.Contains(M_SelectedTargetActor)))
			{
				M_SelectedTargetActor = nullptr;
			}
		}
		
		if (M_SelectedTargetActor == nullptr)
		{
			M_SelectedTargetActor = M_GoalData->SelfTargets.Num() == 0 ? M_GoalData->TeamTargets[0] : M_GoalData->SelfTargets[0];
		}

		M_CanShoot = ChecKWeaponLos(M_SelectedTargetActor) && CheckWithinWeaponRange(M_SelectedTargetActor);
		if (M_CanShoot)
		{
			//Cancel Any MoveCommand if there is one
			M_AIController->StopMovement();
			ShootTarget();
		}
		else if (!M_IsMovingToFiringPosition)
		{
			MoveToFiringPosition();
		}
	}
}

bool UShootBehaviour::IsAmmoInClip()
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

bool UShootBehaviour::CheckPreConditions_Implementation(UBaseGoalData* GoalData)
{
	if (M_GoalOwner == nullptr)
	{
		return false;	
	}
	
	//do I have ammo in clip
	return IsAmmoInClip();
	
}

float UShootBehaviour::GetSelectionScore_Implementation(UBaseGoalData* GoalData)
{
	//TODO add scores here to check for each target in goaldata. target with highest score should become selected target
	return 20;
}

void UShootBehaviour::Initialize_Implementation(AActor* OwnerActor, UBehaviourSelectorComponent* owner)
{
	Super::Initialize_Implementation(OwnerActor, owner);
	M_GoalOwner = Cast<AShooterBaseCharacter>(GoalOwner);
}

void UShootBehaviour::CleanUpPathFollowingDelegate()
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

void UShootBehaviour::OnMoveRequestFinished(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	//Do we need to do anything on success or failure here?
	if (Result.IsSuccess())
	{
	}
	else
	{
	}
	M_IsMovingToFiringPosition = false;
	
	CleanUpPathFollowingDelegate();
}

void UShootBehaviour::MoveToFiringPosition()
{
	M_IsMovingToFiringPosition = true;
	int AcceptanceRadius = 100;

	if (M_AIController)
	{
		M_CanShoot = false;
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
			M_BehaviourState = BehaviourExecutionState::FAILED;
			break;

		case EPathFollowingRequestResult::AlreadyAtGoal:
			OnMoveRequestFinished(RequestResult.MoveId, FPathFollowingResult(EPathFollowingResult::Success, FPathFollowingResultFlags::AlreadyAtGoal));
			break;

		case EPathFollowingRequestResult::RequestSuccessful:
			{
				PathFinishDelegateHandle = PFComponent->OnRequestFinished.AddUObject(this, &UShootBehaviour::OnMoveRequestFinished);
				break;
			}
		default:
			break;
		}
	}
}

void UShootBehaviour::FireWeapon()
{
	if (M_GoalOwner->Implements<UWeaponInterface>())
	{
		IWeaponInterface::Execute_FireWeapon(M_GoalOwner);
	}
}
