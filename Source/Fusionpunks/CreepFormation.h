// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "CreepFormation.generated.h"


UENUM(BlueprintType)
enum class EFormationType : uint8
{
	FT_Circle    UMETA(DisplayName = "CircleHeroFormation"),
	FT_Line		UMETA(DisplayerName = "LineFormation")
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FUSIONPUNKS_API UCreepFormation : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCreepFormation();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	UPROPERTY(EditAnywhere, Category = AIStats)
	EFormationType formationType;

	UPROPERTY(EditDefaultsOnly, Category = AIStats)
		float leaderFollowDistance;

	UPROPERTY(EditDefaultsOnly, Category = AIStats)
		float creepSeparationDistance;

private:
	class AHeroBase* owningHero;

};
