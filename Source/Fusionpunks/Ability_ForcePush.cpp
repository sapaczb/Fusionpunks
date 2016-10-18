// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "Creep.h"
#include "HeroBase.h"
#include "Ability_ForcePush.h"


void AAbility_ForcePush::BeginPlay()
{
	Super::BeginPlay();

	PushRadius = 500.0f;
	PushForce = 2000.0f;
	damage = 10.0f; 
}

void AAbility_ForcePush::Ability()
{
	UE_LOG(LogTemp, Warning, TEXT("Using FORCE PUSH!"));
	FCollisionObjectQueryParams obejctQP;

	obejctQP.AddObjectTypesToQuery(AIHero);
	obejctQP.AddObjectTypesToQuery(Creeps);

	FCollisionQueryParams QueryParameters;
	QueryParameters.AddIgnoredActor(this);
	QueryParameters.OwnerTag = TEXT("Player");

	TArray<FOverlapResult> Results;
	GetWorld()->OverlapMultiByObjectType(Results,GetActorLocation(),FQuat(),obejctQP,FCollisionShape::MakeSphere(PushRadius), QueryParameters);

	//Spawn Effect / Particle System

	if (Results.Num() > 0)
	{
		ACharacter* owner = Cast<ACharacter>(GetOwner());
		if (owner)
		{
			for (int i = 0; i < Results.Num(); i++)
			{
				AHeroBase* enemyHero = Cast<AHeroBase>(GetOwner());
				ACreep* enemyCreep = Cast<ACreep>(Results[i].GetActor());
				if (enemyCreep)
				{
					if (!owner->Tags.Contains(enemyCreep->GetTeam()))
					{
						enemyCreep->TakeDamage(damage, FDamageEvent::FDamageEvent(), owner->GetController(), owner);
						FVector dir = enemyCreep->GetActorLocation() - owner->GetActorLocation();
						dir.Normalize();
						dir *= -1 * PushForce;
						//enemyCreep->GetMesh()->AddForceAtLocation(dir, enemyCreep->GetActorLocation());
						enemyCreep->GetCharacterMovement()->AddRadialImpulse(owner->GetActorLocation(), PushRadius, PushForce, ERadialImpulseFalloff::RIF_Linear, false);
						continue;
					}
				}
				else if(enemyHero)
				{
					enemyHero->TakeDamage(damage, FDamageEvent::FDamageEvent(), owner->GetController(), owner);
					FVector dir = enemyHero->GetActorLocation() - owner->GetActorLocation();
					dir.Normalize();
					dir *= PushForce;
					//enemyCreep->GetMesh()->AddForceAtLocation(dir, enemyCreep->GetActorLocation());
					//enemyHero->GetCharacterMovement()->AddImpulse(owner->GetActorLocation(), true);
					enemyHero->GetCharacterMovement()->AddForce(dir);
					continue;
				}

			}
		}
	}
}