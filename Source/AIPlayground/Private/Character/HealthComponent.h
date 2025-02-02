﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class AIPLAYGROUND_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

	float M_CurrentHealth;
	

public:
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:

	UPROPERTY(EditDefaultsOnly)
	float MaxHealth = 100;

	UPROPERTY(EditDefaultsOnly)
	float TakingDamageThreshold = 20;

	UPROPERTY(EditDefaultsOnly)
	float TakingDamageDecayRatePerSecond = 1;
	
	void ApplyDamage(float Damage);
	float GetHealth();
	float GetHealthPercentage();
	bool GetIsDead();
	bool GetIsTakingDamage();
	float GetMaxHealth();

	float M_CurrentDamageAccumulation = 0;

private:


};
