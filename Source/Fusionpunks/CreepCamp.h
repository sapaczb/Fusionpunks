// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Creep.h"
#include "CreepCamp.generated.h"

UENUM(BlueprintType)
enum class ECampType : uint8
{
	CT_Neutral    UMETA(DisplayName = "Neutral"),
	CT_Cyber      UMETA(DisplayName = "Cyber"),
	CT_Diesel	  UMETA(DisplayName = "Diesel")
};

USTRUCT(BlueprintType)
struct FCaptureVariables
{
	GENERATED_BODY()
	
	FCaptureVariables()
	{
		//Default unless set in blueprint 
		targetCaptureTime = 5.0f;
		captureTimeMultiplier = 1.5f;
		captureTime = targetCaptureTime;
	}
	/*The more creeps that are in the camp the higher the capture time should be
	When a camp is captured all the creeps associated with it should die */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CampVariables)
	float targetCaptureTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CampVariables)
	float captureTimeMultiplier;
	UPROPERTY()
	float captureTime;
	UPROPERTY()
	float cyberCaptureProgress;
	UPROPERTY()
	float dieselCaptureProgress;
	UPROPERTY()
	bool bCyberIsCapturing;
	UPROPERTY()
	bool bDieselIsCapturing;
};

USTRUCT(BlueprintType)
struct FSpawningVariables
{
	GENERATED_BODY()

	FSpawningVariables()
	{
		creepCount = 3;
		creepSpawnTimerTarget = 5.0f;
		creepSpawnTimer = creepSpawnTimerTarget;
		creepSpawnTimerMultiplier = 1.5f;
	}

	/*Spawn rate for creeps is based on how many are currently at the camp... 
	Less = faster spawn rate, More = slower spawn rate*/
	UPROPERTY()
	int creepCount; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CampVariables)
	float creepSpawnTimerTarget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CampVariables)
	float creepSpawnTimerMultiplier;
	UPROPERTY()
	float creepSpawnTimer;
};

UCLASS()
class FUSIONPUNKS_API ACreepCamp : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = Stats)
		ECampType campType;
public:
	UPROPERTY(EditAnywhere, Category = Stats)
	FCaptureVariables captureVariables;
	UPROPERTY(EditAnywhere, Category = Stats)
	FSpawningVariables spawningVariables;
	
	
public:	
	ACreepCamp();
	virtual void BeginPlay() override;
	virtual void Tick( float DeltaSeconds ) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Creeps")
		 TSubclassOf<class ANeutralCreep> neutralCreepRef;

	UPROPERTY(EditDefaultsOnly, Category = "Creeps")
		TSubclassOf<class ACyberCreep> cyberCreepRef;

	UPROPERTY(EditDefaultsOnly, Category = "Creeps")
		TSubclassOf<class ADieselCreep> dieselCreepRef;


//Meshes and triggers 
protected:
	UPROPERTY(EditDefaultsOnly, Category = Appearance)
		UStaticMeshComponent* campMesh; 

	UPROPERTY(EditDefaultsOnly, Category = CollisionComponents)
		USphereComponent* sphereTrigger;

	//mesh for the ring around the camp
	UPROPERTY(EditDefaultsOnly, Category = Appearance)
		UStaticMeshComponent* ringMesh;


//Ring Rotations Stuff
protected:
	UPROPERTY(EditAnywhere, Category = CampVariables)
	float ringRotationSpeed;

	UPROPERTY(EditAnywhere, Category = CampVariables)
	float ringMaterialAlphaSpeed;

	FRotator ringRotation;
	float ringMaterialAlpha;
	bool bCountUp;


//Overlap functions
protected:
	//function for Trigger Events
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* ThisComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	//function for Trigger Exit Events
	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
//Camp Capture Functions
	UFUNCTION(BlueprintCallable, Category = CampFunctions)
		float GetCyberCapturePercentage();

	UFUNCTION(BlueprintCallable, Category = CampFunctions)
		float GetDieselCapturePercentage();


//Creep Spawning Functions
public:
	UFUNCTION(BlueprintCallable, Category = CampFunctions)
		void MinusOneFromCreepCamp();

	void RemoveCreep(ACreep* CreepInCamp);
	void DestroyAllCreeps();



//Creep Spawn Locations
protected:
	TArray<ACreep*> creepArray;

	UPROPERTY(EditAnywhere, Category = SpawnLocation)
		FVector creep1SpawnLocation;

	UPROPERTY(EditAnywhere, Category = SpawnLocation)
		FVector creep2SpawnLocation;

	UPROPERTY(EditAnywhere, Category = SpawnLocation)
		FVector creep3SpawnLocation;

	TArray<FVector> creepSpawnArray;


protected:
	UFUNCTION(BlueprintCallable, Category = CampFunctions)
		void SetToDieselCamp();

	UFUNCTION(BlueprintCallable, Category = CampFunctions)
		void SetToCyberCamp();

	UFUNCTION(BlueprintCallable, Category = CampFunctions)
		void SetToNeutralCamp();
};
