// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Perception/AIPerceptionComponent.h"
#include "DetectionSystem.generated.h"


/**
 * 
 */
UCLASS()
class AIPLAYGROUND_API UDetectionSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	TArray<AActor*> GetAllTeamVisibleActorsOfTeam(int32 Team, int32 QuerierTeam);
	UFUNCTION(BlueprintCallable)
	TArray<AActor*> GetAllTeamVisibleActorsNotOfTeam(int32 Team,int32 QuerierTeam);
	UFUNCTION(BlueprintCallable)
	TArray<AActor*> GetAllTeamVisibleActors(int32 QuerierTeam);

	void RegisterDetectionComponent(class UDetectionComponent* DetectionComponent);
	void UnregisterDetectionComponent(class UDetectionComponent* DetectionComponent);

private:
	TMap<int32, TArray<UDetectionComponent*>> m_DetectionMap;

	
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AIPLAYGROUND_API UDetectionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MyTeam = 0;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	// Sets default values for this component's properties
	UDetectionComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintCallable)
	TArray<AActor*> GetAllVisibleActors();
	UFUNCTION(BlueprintCallable)
	TArray<AActor*> GetAllVisibleActorsOfTeam(int32 team);
	UFUNCTION(BlueprintCallable)
	TArray<AActor*> GetAllVisibleActorsNotOfTeam(int32 team);

	virtual void BeginDestroy() override;

private:
	UPROPERTY()
	AAIController* m_AIController;
	UPROPERTY()
	TArray<AActor*> m_VisibleActors;
	UPROPERTY()
	UDetectionSystem* m_DetectionSystem;

	UFUNCTION()
	void PerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
};



