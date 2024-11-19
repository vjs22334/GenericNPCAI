// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "weaponInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UweaponInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AIPLAYGROUND_API IweaponInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void FireWeapon();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ReloadWeapon();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	float PercentGetAmmoInClipLeftInClip();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int GetAmmoInClip();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int GetAmmo();
	
};
