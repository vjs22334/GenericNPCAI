// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootBehaviour.h"

#include "Character/ShooterBaseCharacter.h"
#include "Character/weaponInterface.h"

void UShootBehaviour::BehaviourEnter_Implementation(UBaseGoalData* GoalData)
{
	
}

void UShootBehaviour::BehaviourExit_Implementation()
{
	
}

void UShootBehaviour::BehaviourTick_Implementation(float DeltaTime)
{
	
}

bool UShootBehaviour::CheckPreConditions_Implementation(UBaseGoalData* GoalData)
{
	AShooterBaseCharacter* goalOwner = Cast<AShooterBaseCharacter>(GoalOwner);
	if (goalOwner == nullptr)
	{
		return false;	
	}
	
	//do I have ammo in clip
	if (goalOwner->Implements<UWeaponInterface>())
	{
		if (IWeaponInterface::Execute_GetAmmoInClip(GoalOwner) > 0)
		{
			return true;
		}
		
	}
	
	return false;
	
}

float UShootBehaviour::GetSelectionScore_Implementation(UBaseGoalData* GoalData)
{
	return Super::GetSelectionScore_Implementation(GoalData);
}

void UShootBehaviour::Initialize_Implementation(AActor* OwnerActor, UBehaviourSelectorComponent* owner)
{
	Super::Initialize_Implementation(OwnerActor, owner);
}
