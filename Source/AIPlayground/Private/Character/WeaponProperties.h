// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WeaponProperties.generated.h"

/**
 * 
 */
UCLASS()
class AIPLAYGROUND_API UWeaponProperties : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly , EditAnywhere)
	float WeaponDamage = 10;
	UPROPERTY(BlueprintReadOnly , EditAnywhere)
	float WeaponAmmoCount = 20;
	UPROPERTY(BlueprintReadOnly , EditAnywhere)
	float WeaponReloadTime = 2;
	UPROPERTY(BlueprintReadOnly , EditAnywhere)
	float WeaponRateOfFire = 0.1;
	UPROPERTY(BlueprintReadOnly , EditAnywhere)
	float WeaponBulletRange = 150;
	UPROPERTY(BlueprintReadOnly , EditAnywhere)
	USkeletalMesh* WeaponMesh;
};
