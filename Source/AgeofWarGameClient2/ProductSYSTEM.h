// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameStruct.h"
#include "AOWGameInstance.h"
#include "GameFramework/Actor.h"
#include "ProductSYSTEM.generated.h"

UCLASS()
class AGEOFWARGAMECLIENT2_API AProductSYSTEM : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProductSYSTEM();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	int monsterIndex;
	UAOWGameInstance* gameInstance;
	FCustomStructSpawnPosition SoldierSpawnPosition;

public:
	UFUNCTION(BlueprintCallable, Category = "TESTPRODUCT")
		void SpawnSolider(int soliderType, int spawnPosition, int enemyType, int GameTime);
	UFUNCTION(BlueprintCallable, Category = "TESTPRODUCT")
		void SpawnNexus(int NexusType, int MonsterIndex);


};
