// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterBaseCharacter.h"

#include "HealthComponent.h"


// Sets default values
AShooterBaseCharacter::AShooterBaseCharacter()
{

	M_HealthComponent = CreateDefaultSubobject<UHealthComponent>(FName("HealthComponent"));
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AShooterBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AShooterBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AShooterBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AShooterBaseCharacter::ApplyDamage_Implementation(float Damage)
{
	M_HealthComponent->ApplyDamage(Damage);
}

float AShooterBaseCharacter::GetHealth_Implementation()
{
	return M_HealthComponent->GetHealth();
}

float AShooterBaseCharacter::GetHealthPercentage_Implementation()
{
	return M_HealthComponent->GetHealthPercentage();
}

bool AShooterBaseCharacter::GetIsDead_Implementation()
{
	return M_HealthComponent->GetIsDead();
}

float AShooterBaseCharacter::GetMaxHealth_Implementation()
{
	return M_HealthComponent->GetMaxHealth();
}

