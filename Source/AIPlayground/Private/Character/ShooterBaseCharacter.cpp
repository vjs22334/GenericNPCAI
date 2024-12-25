// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterBaseCharacter.h"

#include "AIController.h"
#include "HealthComponent.h"
#include "WeaponComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
AShooterBaseCharacter::AShooterBaseCharacter()
{

	M_HealthComponent = CreateDefaultSubobject<UHealthComponent>(FName("HealthComponent"));
	M_WeaponComponent = CreateDefaultSubobject<UWeaponComponent>(FName("WeaponComponent"));
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

	if (M_HealthComponent->GetIsDead())
	{
		Destroy();
	}
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

void AShooterBaseCharacter::FireWeapon_Implementation()
{
	M_WeaponComponent->FireWeapon();
}

int AShooterBaseCharacter::GetAmmo_Implementation()
{
	return M_WeaponComponent->GetAmmoInClip();
}

int AShooterBaseCharacter::GetAmmoInClip_Implementation()
{
	return M_WeaponComponent->GetAmmoInClip();
}

float AShooterBaseCharacter::GetPercentAmmoLeftInClip_Implementation()
{
	return M_WeaponComponent->GetPercentAmmoLeftInClip();
}

void AShooterBaseCharacter::ReloadWeapon_Implementation()
{
	M_WeaponComponent->ReloadWeapon();
}

bool AShooterBaseCharacter::CanBeSeenFrom(const FVector& ObserverLocation, FVector& OutSeenLocation,
	int32& NumberOfLoSChecksPerformed, float& OutSightStrength, const AActor* IgnoreActor, const bool* bWasVisible,
	int32* UserData) const
{
	FHitResult OutHit;
	FVector SightTargetLocation = this->GetMesh()->GetSocketLocation(FName("neck_01"));

	bool hit = GetWorld()->LineTraceSingleByChannel(OutHit, ObserverLocation, SightTargetLocation,ECC_Visibility,FCollisionQueryParams(FName("LOSTrace"),false,IgnoreActor));

	if (!hit || (IsValid(OutHit.GetActor()) && OutHit.GetActor()->IsOwnedBy(this)))
	{
		OutSeenLocation = SightTargetLocation;
		OutSightStrength = 1;
		return true;
	}

	
	OutSightStrength = 0;
	return false;
}

FRotator AShooterBaseCharacter::GetBaseAimRotation() const
{
	if (!IsPlayerControlled())
	{
		if (AAIController* aiController = Cast<AAIController>(GetController()))
		{
		
			FVector focalPoint = aiController->GetFocalPoint();
			if (focalPoint != FAISystem::InvalidLocation)
			{
				return UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(),focalPoint);

			}
		}
	}
	return Super::GetBaseAimRotation();
}

UWeaponProperties* AShooterBaseCharacter::GetWeaponProperties_Implementation()
{
	return M_WeaponComponent->GetWeaponProperties();
}

bool AShooterBaseCharacter::GetIsTakingDamage_Implementation()
{
	return M_HealthComponent->GetIsTakingDamage();
}

