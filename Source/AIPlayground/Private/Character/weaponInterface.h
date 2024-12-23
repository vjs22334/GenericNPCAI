// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "weaponInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(Blueprintable)
class UWeaponInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AIPLAYGROUND_API IWeaponInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void FireWeapon();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ReloadWeapon();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	float GetPercentAmmoLeftInClip();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int GetAmmoInClip();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int GetAmmo();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UWeaponProperties* GetWeaponProperties();
	
};
