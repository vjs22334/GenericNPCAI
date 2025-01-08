// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootBehaviour.h"

#include "AIController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Character/ShooterBaseCharacter.h"
#include "Character/weaponInterface.h"
#include "UtilityAI/GoalGenerators/CombatGoalData.h"
#include "EnvironmentQuery/EnvQueryManager.h"

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

	M_StrafeDelay = FMath::RandRange(MinStrafeDelay, MaxStrafeDelay);
	M_TimeElapsedSinceShootStart = FMath::RandRange(MinStrafeDelay, MaxStrafeDelay);
	
}

void UShootBehaviour::BehaviourExit_Implementation()
{
	Super::BehaviourExit_Implementation();
	if (M_AIController != nullptr)
	{
		M_AIController->StopMovement();
		M_AIController->K2_ClearFocus();
	}
	M_SelectedTargetActor = nullptr;
	M_GoalData = nullptr;
	M_AIController = nullptr;
}

void UShootBehaviour::ShootTarget()
{
	if (AShooterBaseCharacter* targetCharacter = Cast<AShooterBaseCharacter>(M_SelectedTargetActor))
	{
		M_AIController->SetFocalPoint(targetCharacter->GetMesh()->GetBoneLocation("spine_01") + AimOffset);
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
	FVector startLocation = M_GoalOwner->GetMesh()->GetSocketLocation("neck_01");

	if (AShooterBaseCharacter* targetCharacter = Cast<AShooterBaseCharacter>(TargetActor))
	{
		 endLocation = targetCharacter->GetMesh()->GetSocketLocation("neck_01");
	}
	
	bool hit = GetWorld()->LineTraceSingleByChannel(OutHit, startLocation, endLocation,ECC_Visibility,FCollisionQueryParams(FName("LOSTrace"),false,M_GoalOwner));
	// DrawDebugLine(
	// 		GetWorld(),
	// 		startLocation,
	// 		endLocation,
	// 		FColor(255, 0, 0),
	// 		false, -1, 0,
	// 		5
	//	);
	if (!hit || (IsValid(OutHit.GetActor()) && (OutHit.GetActor()->IsOwnedBy(TargetActor) || OutHit.GetActor()->IsOwnedBy(M_GoalOwner))))
	{
		return true;
	}
	return false;
}

bool UShootBehaviour::CheckWithinWeaponRange(AActor* TargetActor)
{
	FVector rangeToTarget = TargetActor->GetActorLocation() - M_GoalOwner->GetActorLocation();
	if(rangeToTarget.Length() <= M_WeaponRange)
	{
		return true;
	}
	return false;
}

void UShootBehaviour::RefreshTargetLists(UCombatGoalData* GoalData)
{
	if (GoalData == nullptr)
	{
		return;
	}
	M_AllVisibleTargets.Empty();
	M_AllVisibleTargets.Append(GoalData->SelfTargets);
	M_AllVisibleTargets.Append(GoalData->TeamTargets);

	M_ShootableTargets.Empty();
	for (AActor* Target : M_AllVisibleTargets)
	{
		if (ChecKWeaponLos(Target) && CheckWithinWeaponRange(Target))
		{
			M_ShootableTargets.Add(Target);
		}
	}
}

void UShootBehaviour::BehaviourTick_Implementation(float DeltaTime)
{
	Super::BehaviourTick_Implementation(DeltaTime);
	if (M_GoalData == nullptr || ( M_GoalData->SelfTargets.Num() == 0 && M_GoalData->TeamTargets.Num() == 0))
	{
		//we have no visible targets
		M_BehaviourState = BehaviourExecutionState::COMPLETED;
	}
	else if (!IsAmmoInClip())
	{
		//we are out of ammo
		M_BehaviourState = BehaviourExecutionState::COMPLETED;
	}
	else
	{
		
		RefreshTargetLists(M_GoalData);
		if (M_ShootableTargets.Num() == 0)
		{
			M_BehaviourState = BehaviourExecutionState::COMPLETED;
			return;
		}
		
		if (M_SelectedTargetActor != nullptr)
		{
			if (!M_ShootableTargets.Contains(M_SelectedTargetActor))
			{
				M_SelectedTargetActor = nullptr;
			}
		}
		
		if (M_SelectedTargetActor == nullptr)
		{
			// select a new target
			int i = FMath::RandRange(0,M_ShootableTargets.Num()-1);
			M_SelectedTargetActor = M_ShootableTargets[i];
		}

		M_CanShoot = ChecKWeaponLos(M_SelectedTargetActor) && CheckWithinWeaponRange(M_SelectedTargetActor);
		if (M_CanShoot)
		{
			ShootTarget();
		}

		if (M_TimeElapsedSinceShootStart > M_StrafeDelay)
		{
			RunStrafeEQS();
		}
		else
		{
			M_TimeElapsedSinceShootStart += DeltaTime;
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
	if (!IsAmmoInClip())
	{
		return false;
	}

	//DO we have a target in weapon range?
	if (UCombatGoalData* CombatGoalData = Cast<UCombatGoalData>(GoalData))
	{
		RefreshTargetLists(CombatGoalData);
		return M_ShootableTargets.Num() > 0;
	}
	
	return false;

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
	if (M_GoalOwner->Implements<UWeaponInterface>())
	{
		UWeaponProperties* weapondata = IWeaponInterface::Execute_GetWeaponProperties(M_GoalOwner);
		M_WeaponRange = weapondata->WeaponBulletRange;
	}
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
	M_TimeElapsedSinceShootStart = 0;
	CleanUpPathFollowingDelegate();
}

void UShootBehaviour::RunStrafeEQS()
{
	M_IsMovingToFiringPosition = true;
	M_GoalOwner->TargetToAttack = M_SelectedTargetActor;
	M_AIController->SetFocus(M_SelectedTargetActor);

	FEnvQueryRequest StrafeSpotQueryRequest = FEnvQueryRequest(StrafeEQS, M_GoalOwner);
	StrafeSpotQueryRequest.Execute(EEnvQueryRunMode:: RandomBest25Pct, this,&UShootBehaviour::HandleQueryResult);
	
}

void UShootBehaviour::HandleQueryResult(TSharedPtr<FEnvQueryResult> EnvQueryResult)
{
	if (EnvQueryResult->FEnvQueryResult::IsSuccessful())
	{
		M_LocationToStrafeTo = EnvQueryResult->GetItemAsLocation(0);
		DrawDebugSphere(GetWorld(), M_LocationToStrafeTo, 50, 16, FColor(0, 0, 225), false, -1, 0, 5);
		if (M_LocationToStrafeTo != FVector(0, 0, 0))
		{
			if (M_AIController)
			{
				M_CanShoot = false;

				UPathFollowingComponent* PFComponent = M_AIController->GetPathFollowingComponent();
				FAIMoveRequest MoveReq;
				MoveReq.SetAllowPartialPath(true);
				MoveReq.SetAcceptanceRadius(20);
				MoveReq.SetCanStrafe(true);
				MoveReq.SetReachTestIncludesAgentRadius(true);
				MoveReq.SetReachTestIncludesGoalRadius(true);
				MoveReq.SetUsePathfinding(true);
				MoveReq.SetGoalLocation(M_LocationToStrafeTo);

				const FPathFollowingRequestResult RequestResult = M_AIController->MoveTo(MoveReq);
				switch (RequestResult.Code)
				{
				case EPathFollowingRequestResult::Failed:
					M_IsMovingToFiringPosition = false;
					M_SelectedTargetActor = nullptr;
					M_TimeElapsedSinceShootStart = 0;
					break;

				case EPathFollowingRequestResult::AlreadyAtGoal:
					OnMoveRequestFinished(RequestResult.MoveId,
					                      FPathFollowingResult(EPathFollowingResult::Success,
					                                           FPathFollowingResultFlags::AlreadyAtGoal));
					break;

				case EPathFollowingRequestResult::RequestSuccessful:
					{
						PathFinishDelegateHandle = PFComponent->OnRequestFinished.AddUObject(
							this, &UShootBehaviour::OnMoveRequestFinished);
						break;
					}
				default:
					break;
				}
			}
		}
	}
	else
	{
		M_TimeElapsedSinceShootStart = 0;
	}

}

void UShootBehaviour::FireWeapon()
{
	if (M_GoalOwner->Implements<UWeaponInterface>())
	{
		IWeaponInterface::Execute_FireWeapon(M_GoalOwner);
	}
}
