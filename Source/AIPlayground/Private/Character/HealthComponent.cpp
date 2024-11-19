﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"


// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	M_CurrentHealth = MaxHealth;

	// ...
	
}

void UHealthComponent::ApplyDamage(float Damage)
{
	M_CurrentHealth -= Damage;
	if (M_CurrentHealth < 0)
	{
		M_CurrentHealth = 0;
	}
}

float UHealthComponent::GetHealth()
{
	return M_CurrentHealth;
}

float UHealthComponent::GetHealthPercentage()
{
	if (MaxHealth <= 0)
	{
		return 0;
	}
	return (M_CurrentHealth / MaxHealth)*100;
}

bool UHealthComponent::GetIsDead()
{
	return M_CurrentHealth == 0;
}

float UHealthComponent::GetMaxHealth()
{
	return MaxHealth;
}


