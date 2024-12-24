// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HealthSystemInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(Blueprintable)
class UHealthSystemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AIPLAYGROUND_API IHealthSystemInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	float GetHealth();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	float GetMaxHealth();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	float GetHealthPercentage();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ApplyDamage(float Damage);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool GetIsDead();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool GetIsTakingDamage();
	
};
