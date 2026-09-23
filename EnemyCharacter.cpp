// Fill out your copyright notice in the Description page of Project Settings.
#include "EnemyCharacter.h"
#include "MainCharacter.h"
#include "Components/SphereComponent.h"
#include "EnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"

AEnemyCharacter::AEnemyCharacter()
	: Super()
{
	PrimaryActorTick.bCanEverTick = true;

	AgroSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AgroSphere"));
	AgroSphere->SetupAttachment(GetRootComponent());

	AttackSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AttackSphere"));
	AttackSphere->SetupAttachment(GetRootComponent());

	Health = 20.f;
	MaxHealth = 20.f;
	Damage = 100.f;
	XP = 10;
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	PatrolPoints.Add(PatrolPoint1 + GetActorLocation());
	PatrolPoints.Add(PatrolPoint2 + GetActorLocation());
	PatrolPoints.Add(PatrolPoint3 + GetActorLocation());
	PatrolPoints.Add(PatrolPoint4 + GetActorLocation());

	AgroSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::AgroSphereBeginOverlap);
	AgroSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemyCharacter::AgroSphereEndOverlap);

	AttackSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::AttackSphereBeginOverlap);
	AttackSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemyCharacter::AttackSphereEndOverlap);
}

bool AEnemyCharacter::GetNextPatrolLocation(FVector& OutLocation)
{
	if (PatrolPoints.Num() > 0)
	{
		if (CurrentPatrolIndex >= PatrolPoints.Num())
		{
			CurrentPatrolIndex = 0;
		}
		OutLocation = PatrolPoints[CurrentPatrolIndex++];
		return true;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("No patrol points set for ") + GetName());
	}
	return false;
}

bool AEnemyCharacter::GetRandomReachableLocation(FVector& OutLocation)
{
	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	if (!NavSys) return false;

	FNavLocation RandomLocation;
	// Pick a random point on the nav mesh within RandomWanderRadius of the AI's current position
	if (NavSys->GetRandomReachablePointInRadius(GetActorLocation(), RandomWanderRadius, RandomLocation))
	{
		OutLocation = RandomLocation.Location;
		return true;
	}
	return false;
}

void AEnemyCharacter::AgroSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		if (AMainCharacter* Main = Cast<AMainCharacter>(OtherActor))
		{
			if (AEnemyController* EnemyController = Cast<AEnemyController>(GetController()))
			{
				EnemyController->GetBlackboardComponent()->SetValueAsObject(TEXT("TargetActor"), Main);
				// notify the player they're being chased
				Main->IncrementChaseCount();
			}
		}
	}
}

void AEnemyCharacter::AgroSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		if (AMainCharacter* Main = Cast<AMainCharacter>(OtherActor))
		{
			if (AEnemyController* EnemyController = Cast<AEnemyController>(GetController()))
			{
				EnemyController->GetBlackboardComponent()->ClearValue(TEXT("TargetActor"));
				// notify the player this enemy stopped chasing them
				Main->DecrementChaseCount();
			}
		}
	}
}

void AEnemyCharacter::AttackSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		if (AMainCharacter* Main = Cast<AMainCharacter>(OtherActor))
		{
			if (AEnemyController* EnemyController = Cast<AEnemyController>(GetController()))
			{
				EnemyController->GetBlackboardComponent()->SetValueAsBool(TEXT("InAttackRange"), true);
			}
		}
	}
}

void AEnemyCharacter::AttackSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		if (AMainCharacter* Main = Cast<AMainCharacter>(OtherActor))
		{
			if (AEnemyController* EnemyController = Cast<AEnemyController>(GetController()))
			{
				EnemyController->GetBlackboardComponent()->SetValueAsBool(TEXT("InAttackRange"), false);
			}
		}
	}
}

void AEnemyCharacter::DeathEnd()
{
	Super::DeathEnd();
	//Destroy();
}

void AEnemyCharacter::Die(AActor* Causer)
{
	Super::Die(Causer);

	AgroSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AttackSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (AEnemyController* EnemyController = Cast<AEnemyController>(GetController()))
	{
		EnemyController->StopBehaviorTree();
	}

	if (AMainCharacter* Main = Cast<AMainCharacter>(Causer))
	{
		Main->AddXP(XP);
	}
}