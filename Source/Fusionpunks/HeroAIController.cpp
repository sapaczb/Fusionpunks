// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "HeroBase.h"
#include "HeroAIController.h"

AHeroAIController::AHeroAIController()
{
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnsensingComponent"));
	
	const ConstructorHelpers::FObjectFinder<UBehaviorTree> BTFinder(TEXT("BehaviorTree'/Game/Heroes/AI/HeroAI_Tree.HeroAI_Tree'"));

	if (IsValid(BTFinder.Object))
	{
		BehaviorTreeAsset = BTFinder.Object;
	}
	bAttachToPawn = true;
	
}
void AHeroAIController::Possess(APawn* Pawn)
{
	Super::Possess(Pawn);
	
	if (BehaviorTreeAsset)
	{
		BlackboardComponent->InitializeBlackboard(*BehaviorTreeAsset->BlackboardAsset);
		//UseBlackboard(BehaviorTreeAsset->BlackboardAsset, BlackboardComponent);
		BehaviorTreeComponent->StartTree(*BehaviorTreeAsset);
		
		
	}
}

void AHeroAIController::BeginPlay() 
{
	Super::BeginPlay();
	TArray<AActor*> actorList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), creepCampClass, actorList);
	for (int32 i = 0; i < actorList.Num(); i++)
	{
		creepCamps.Add(Cast<ACreepCamp>(actorList[i]));
	}
	UE_LOG(LogTemp, Log, TEXT("Found %d CreepCamps."), creepCamps.Num());
	campPriorityList.GeneratePriorityList(creepCamps, GetPawn());
	campPriorityList.DisplayDistances();

	
	
	TArray<AActor*> bases;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), baseClass, bases);
	if (bases.Num() > 0)
	{
		UE_LOG(LogTemp, Display, TEXT("BASE FOUND"));
		BlackboardComponent->SetValueAsObject("Home", bases[0]);
		
	}
	BlackboardComponent->SetValueAsBool("ReachedCamp", false);
	BlackboardComponent->SetValueAsBool("CapturedCamp", true);
	
}

void AHeroAIController::ResetAITreeTaskStatus() 
{
	BlackboardComponent->SetValueAsBool("ReachedCamp", false);
	BlackboardComponent->SetValueAsBool("CapturedCamp", true);
	ResetAllCampsSafetyStatus();
}


void AHeroAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UpdateControlRotation(DeltaTime, false);
}

TArray<ACreepCamp*> AHeroAIController::GetCreepCampList() 
{
	campPriorityList.SortByDistance(GetPawn());
	return campPriorityList.creepCampList;
}

void AHeroAIController::ResetAllCampsSafetyStatus() 
{
	campPriorityList.ResetCampsSafetyStatus();
}
TArray<ACreepCamp*> AHeroAIController::GetSortedOwnedCampList() 
{
	campOwnedPriorityList.EmptyList();
	campOwnedPriorityList.GeneratePriorityList(Cast<AHeroBase>(GetPawn())->GetCapturedCamps(), GetPawn());
	return campOwnedPriorityList.creepCampList;

}
void AHeroAIController::ResetAllCampsRecruitStatus()
{
	campPriorityList.ResetCampsRecruitedStatus();
}