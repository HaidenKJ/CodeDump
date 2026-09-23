// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class LAB1_KAYMENJ_API AEnemyCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AEnemyCharacter();

	//Used to chase the player
	UPROPERTY(VisibleAnywhere, Category = "BehaviorTree")
	TObjectPtr<class USphereComponent> AgroSphere;

	UFUNCTION()
	void AgroSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void AgroSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	// Used to attack the player
	UPROPERTY(VisibleAnywhere, Category = "BehaviorTree")
	TObjectPtr<class USphereComponent> AttackSphere;

	UFUNCTION()
	void AttackSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void AttackSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, Category = "Patrol", Meta = (MakeEditWidget))
	FVector PatrolPoint1;
	UPROPERTY(EditAnywhere, Category = "Patrol", Meta = (MakeEditWidget))
	FVector PatrolPoint2;
	UPROPERTY(EditAnywhere, Category = "Patrol", Meta = (MakeEditWidget))
	FVector PatrolPoint3;
	UPROPERTY(EditAnywhere, Category = "Patrol", Meta = (MakeEditWidget))
	FVector PatrolPoint4;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Patrol")
	int32 RandomMoveCount = 3;

	bool GetNextPatrolLocation(FVector& OutLocation);
	bool GetRandomReachableLocation(FVector& OutLocation);

	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<class UBehaviorTree> BehaviorTree;

	UPROPERTY(EditAnywhere, Category = "AI")
	float RandomWanderRadius = 1000.f;

	TArray<FVector> PatrolPoints;
	int32 CurrentPatrolIndex = 0;

public: 
	virtual void Die(AActor* Causer) override;
	virtual void DeathEnd() override;
};