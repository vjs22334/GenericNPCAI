// Fill out your copyright notice in the Description page of Project Settings.


#include "DetectionSystem.h"

#include "AIController.h"
#include "Character/HealthSystemInterface.h"
#include "GameFramework/Character.h"
#include "Perception/AISense_Sight.h"


TArray<AActor*> UDetectionSystem::GetAllTeamVisibleActorsOfTeam(int32 Team, int32 QuerierTeam)
{
	TArray<AActor*> Result;
	if (m_DetectionMap.Contains(QuerierTeam))
	{
		for (int32 i = 0; i < m_DetectionMap[QuerierTeam].Num(); i++)
		{
			for(AActor* actor : m_DetectionMap[QuerierTeam][i]->GetAllVisibleActorsOfTeam(Team))
			{
				Result.AddUnique(actor);
			}
		}
	}
	return Result;
}

TArray<AActor*> UDetectionSystem::GetAllTeamVisibleActorsNotOfTeam(int32 Team, int32 QuerierTeam)
{
	TArray<AActor*> Result;
	if (TArray<UDetectionComponent*>* DetectionComponents = m_DetectionMap.Find(QuerierTeam))
	{
		for (int32 i = 0; i < DetectionComponents->Num(); i++)
		{
			for(AActor* actor : (*DetectionComponents)[i]->GetAllVisibleActorsNotOfTeam(Team))
			{
				Result.AddUnique(actor);
			}
		}
	}
	return Result;
}

TArray<AActor*> UDetectionSystem::GetAllTeamVisibleActors(int32 QuerierTeam)
{
	TArray<AActor*> Result;
	if (TArray<UDetectionComponent*>* DetectionComponents = m_DetectionMap.Find(QuerierTeam))
	{
		for (int32 i = 0; i < DetectionComponents->Num(); i++)
		{
			for(AActor* actor : (*DetectionComponents)[i]->GetAllVisibleActors())
			{
				Result.AddUnique(actor);
			}
		}
	}
	return Result;
}

void UDetectionSystem::RegisterDetectionComponent(class UDetectionComponent* DetectionComponent)
{
	int32 Team = DetectionComponent->MyTeam;
	TArray<UDetectionComponent*>& DetectionComponents = m_DetectionMap.FindOrAdd(Team);
	DetectionComponents.Add(DetectionComponent);
}

void UDetectionSystem::UnregisterDetectionComponent(class UDetectionComponent* DetectionComponent)
{
	int32 Team = DetectionComponent->MyTeam;
	if (TArray<UDetectionComponent*>* DetectionComponents = m_DetectionMap.Find(Team))
	{
		DetectionComponents->Remove(DetectionComponent);
	}
}

UDetectionComponent::UDetectionComponent()
{

	PrimaryComponentTick.bCanEverTick = true;

}


void UDetectionComponent::BeginPlay()
{
	Super::BeginPlay();

	ACharacter* Character = Cast<ACharacter>(this->GetOwner());
	if (Character != nullptr)
	{
		m_AIController = Cast<AAIController>(Character->GetController());
		if (m_AIController != nullptr)
		{
			UAIPerceptionComponent* PerceptionComponent = m_AIController->GetPerceptionComponent();
			if (PerceptionComponent)
			{
				PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &UDetectionComponent::PerceptionUpdated);

				if (UWorld* World = GetWorld())
				{
					m_DetectionSystem = World->GetGameInstance()->GetSubsystem<UDetectionSystem>();
					if (m_DetectionSystem != nullptr)
					{
						m_DetectionSystem->RegisterDetectionComponent(this);
					}
				}
			}
		}
	}
	
	
}


void UDetectionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	for (int i = 0; i < m_VisibleActors.Num(); i++)
	{
		if (m_VisibleActors[i] == nullptr)
		{
			m_VisibleActors.RemoveAt(i);
		}
		else if (m_VisibleActors[i]->Implements<UHealthSystemInterface>())
		{
			if (IHealthSystemInterface::Execute_GetIsDead(m_VisibleActors[i]))
			m_VisibleActors.RemoveAt(i);
		}
	}
}


TArray<AActor*> UDetectionComponent::GetAllVisibleActors()
{
	return m_VisibleActors;
}

TArray<AActor*> UDetectionComponent::GetAllVisibleActorsOfTeam(int32 Team)
{
	TArray<AActor*> result;
	for (int32 i = 0; i < m_VisibleActors.Num(); i++)
	{
		if (m_VisibleActors[i] != nullptr)
		{
			if (UDetectionComponent* DetectionComponent =  Cast<UDetectionComponent>(m_VisibleActors[i]->GetComponentByClass(UDetectionComponent::StaticClass())))
			{
				if (DetectionComponent->MyTeam == Team)
				{
					result.Add(m_VisibleActors[i]);
				}
			}
		}
	}
	return result;
}

TArray<AActor*> UDetectionComponent::GetAllVisibleActorsNotOfTeam(int32 Team)
{
	TArray<AActor*> result;
	for (int32 i = 0; i < m_VisibleActors.Num(); i++)
	{
		if (m_VisibleActors[i] != nullptr)
		{
			if (UDetectionComponent* DetectionComponent = Cast<UDetectionComponent>(m_VisibleActors[i]->GetComponentByClass(UDetectionComponent::StaticClass())))
			{
				if (DetectionComponent->MyTeam != Team)
				{
					result.Add(m_VisibleActors[i]);
				}
			}
		}
	}
	return result;
}

void UDetectionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (m_DetectionSystem != nullptr)
	{
		m_DetectionSystem->UnregisterDetectionComponent(this);
	}
}

void UDetectionComponent::PerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (UDetectionComponent* DetectionComponent = Cast<UDetectionComponent>(Actor->GetComponentByClass(UDetectionComponent::StaticClass())))
	{
		if (Stimulus.Type == UAISense::GetSenseID<UAISense_Sight>())
		{
			if (Stimulus.WasSuccessfullySensed())
			{
				m_VisibleActors.Add(Actor);
			}
			else
			{
				m_VisibleActors.Remove(Actor);
			}
		}
	}
}




