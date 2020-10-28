// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AOWGameInstance.h"
#include "GameStruct.h"
#include "Dom/JsonObject.h"
#include "GameFramework/Actor.h"
#include "GameSocket.h"
#include "AutoGameTime.h"
#include "GameNetworking.generated.h"

UCLASS()
class AGEOFWARGAMECLIENT2_API AGameNetworking : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameNetworking();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAMENETWORK")
		FString networkid;
		
public:
	UFUNCTION(BlueprintCallable, Category = "GAMENETWORK")
		void ConnGameServer(); // ���� ���� ����� �ʱ�ȭ �۾�  
	UFUNCTION(BlueprintCallable, Category = "GAMENETWORK")
		void GameReadyComplete(); // ���� �غ�Ϸ�  
	UFUNCTION(BlueprintCallable, Category = "GAMENETWORK")
		void ProductMonsterBear(int SpawnPosition,int SpawnTime); // ���� ���� �̺�Ʈ Bear
	UFUNCTION(BlueprintCallable, Category = "GAMENETWORK")
		void ProductMonsterMurdock(int spawnPosition, int SpawnTime);

public:
	class GameSocket* GameSocketObject = nullptr;
	FRunnableThread* CurrentThread = nullptr;
	class AutoGameTime* AutoGameTimeObject = nullptr;
	FRunnableThread* CurrentThread2 = nullptr;
	FTimerHandle GameTimerHandler; // ���� �ð� Ÿ�̸� 
	UAOWGameInstance* gameInstance; // ���� �ν��Ͻ� 

public:




};
