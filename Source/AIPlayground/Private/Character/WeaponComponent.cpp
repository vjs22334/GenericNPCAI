﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponComponent.h"


void UWeaponComponent::SetupWeapon_Implementation()
{
	
}

float UWeaponComponent::GetPercentAmmoLeftInClip_Implementation()
{
	return  0;
}

void UWeaponComponent::FireWeapon_Implementation()
{
	
}
void UWeaponComponent::ReloadWeapon_Implementation()
{
	
}

int UWeaponComponent::GetAmmoInClip_Implementation()
{
	return  0;
}

UWeaponProperties* UWeaponComponent::GetWeaponProperties_Implementation()
{
	return WeaponData;
}



// Sets default values for this component's properties
UWeaponComponent::UWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
	SetupWeapon();
	
}


// Called every frame
void UWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


