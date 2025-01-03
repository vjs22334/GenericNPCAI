// Fill out your copyright notice in the Description page of Project Settings.


#include "UEnvQueryGenerator_GridCustom.h"
#include "EnvironmentQuery/Contexts/EnvQueryContext_Querier.h"

#define LOCTEXT_NAMESPACE "EnvQueryGenerator"
void UUEnvQueryGenerator_GridCustom::GenerateItems(FEnvQueryInstance& QueryInstance) const
{
	UObject* BindOwner = QueryInstance.Owner.Get();
	GridSizeX.BindData(BindOwner, QueryInstance.QueryID);
	GridSizeY.BindData(BindOwner, QueryInstance.QueryID);
	SpaceBetween.BindData(BindOwner, QueryInstance.QueryID);

	float LengthValue = GridSizeX.GetValue();
	float WidthValue = GridSizeX.GetValue();
	float DensityValue = SpaceBetween.GetValue();

	const int32 ItemCountX = FPlatformMath::TruncToInt((LengthValue * 2.0f / DensityValue) + 1);
	const int32 ItemCountHalfX = ItemCountX / 2;

	const int32 ItemCountY = FPlatformMath::TruncToInt((WidthValue * 2.0f / DensityValue) + 1);
	const int32 ItemCountHalfY = ItemCountX / 2;

	TArray<FVector> ContextLocations;
	QueryInstance.PrepareContext(GenerateAround, ContextLocations);

	TArray<FNavLocation> GridPoints;
	GridPoints.Reserve(ItemCountX * ItemCountY * ContextLocations.Num());

	for (int32 ContextIndex = 0; ContextIndex < ContextLocations.Num(); ContextIndex++)
	{
		for (int32 IndexX = 0; IndexX < ItemCountX; ++IndexX)
		{
			for (int32 IndexY = 0; IndexY < ItemCountY; ++IndexY)
			{
				const FNavLocation TestPoint = FNavLocation(ContextLocations[ContextIndex] - FVector(DensityValue * (IndexX - ItemCountHalfX), DensityValue * (IndexY - ItemCountHalfY), 0));
				GridPoints.Add(TestPoint);
			}
		}
	}

	ProjectAndFilterNavPoints(GridPoints, QueryInstance);
	StoreNavPoints(GridPoints, QueryInstance);
}

FText UUEnvQueryGenerator_GridCustom::GetDescriptionTitle() const
{
	return FText::Format(LOCTEXT("CustomGridDescriptionGenerateAroundContext", "{0}: generate around {1}"),
		Super::GetDescriptionTitle(), UEnvQueryTypes::DescribeContext(GenerateAround));
}

FText UUEnvQueryGenerator_GridCustom::GetDescriptionDetails() const
{
	FText Desc = FText::Format(LOCTEXT("CustomGridDescription", "GridX: {0},{1} space between: {2}"),
		FText::FromString(GridSizeX.ToString()),FText::FromString(GridSizeY.ToString()), FText::FromString(SpaceBetween.ToString()));

	FText ProjDesc = ProjectionData.ToText(FEnvTraceData::Brief);
	if (!ProjDesc.IsEmpty())
	{
		FFormatNamedArguments ProjArgs;
		ProjArgs.Add(TEXT("Description"), Desc);
		ProjArgs.Add(TEXT("ProjectionDescription"), ProjDesc);
		Desc = FText::Format(LOCTEXT("CustomGridDescriptionWithProjection", "{Description}, {ProjectionDescription}"), ProjArgs);
	}

	return Desc;
}
#undef LOCTEXT_NAMESPACE
