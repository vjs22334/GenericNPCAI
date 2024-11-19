// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HealthComponent.h"
#include "HealthSystemInterface.h"
#include "weaponInterface.h"
#include "WeaponComponent.h"
#include "GameFramework/Character.h"
#include "ShooterBaseCharacter.generated.h"

UCLASS()
class AIPLAYGROUND_API AShooterBaseCharacter : public ACharacter, public IHealthSystemInterface
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = Health)
	UHealthComponent* M_HealthComponent;

public:
	// Sets default values for this character's properties
	AShooterBaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void ApplyDamage_Implementation(float Damage) override;
	virtual float GetHealth_Implementation() override;
	virtual float GetHealthPercentage_Implementation() override;
	virtual bool GetIsDead_Implementation() override;
	virtual float GetMaxHealth_Implementation() override;
};
