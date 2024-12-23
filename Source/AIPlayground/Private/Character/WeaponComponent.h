// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponProperties.h"
#include "Components/ActorComponent.h"
#include "WeaponComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) , Blueprintable)
class AIPLAYGROUND_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

	
public:
	// Sets default values for this component's properties
	UWeaponComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UWeaponProperties* WeaponData;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetupWeapon();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void FireWeapon();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ReloadWeapon();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	float GetPercentAmmoLeftInClip();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int GetAmmoInClip();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UWeaponProperties* GetWeaponProperties();
};
