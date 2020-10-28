// Fill out your copyright notice in the Description page of Project Settings.


#include "GameNetworking.h"
#include "Engine/Engine.h"
#include "Serialization/JsonReader.h"
#include "FSocketMessageHeader.h"
#include "Serialization/JsonSerializer.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Engine/BlueprintGeneratedClass.h"
#include "string"
#include "GameSocket.h"
#include "AOWGameInstance.h"


// Sets default values
AGameNetworking::AGameNetworking()
{
	PrimaryActorTick.bCanEverTick = false;
	//gmeInstance = Cast<UAOWGameInstance>(UGameplayStatics::GetGameInstance(GEngine->GetWorld())); 
	//gameInstance = GEngine->GetWorld()->GetGameInstance<UAOWGameInstance>(); // ���� ��忡���� �ƾ��� �ֳ�? ���尡 �����ϰ� null ���� �ƴϿ����ϱ� 
}

// Called when the game starts or when spawned
void AGameNetworking::BeginPlay()
{
	gameInstance = Cast<UAOWGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())); // �����ν��Ͻ� �ʱ�ȭ 
}

// Called every frame
void AGameNetworking::Tick(float DeltaTime)
{

}

// ���� ���� ������ run �� ���� 
void AGameNetworking::ConnGameServer()
{
	if (!CurrentThread && FPlatformProcess::SupportsMultithreading())
	{

		// ���� Ÿ�� ������ run
		AutoGameTimeObject = new AutoGameTime(gameInstance);
		CurrentThread2 = FRunnableThread::Create(AutoGameTimeObject, TEXT("AutoTimeThread"));

		// ���� ���� ������ run 
		GameSocketObject = new GameSocket(gameInstance, AutoGameTimeObject);
		CurrentThread = FRunnableThread::Create(GameSocketObject, TEXT("SocketThread"));

		//GetWorldTimerManager().SetTimer(GameTimerHandler, this, &AGameNetworking::RunningTime, 2.0f, true, 1.0f); // 1�ʵ� 2�ʸ��� �̺�Ʈ �Լ� ���� 
		//GetWorldTimerManager().PauseTimer(GameTimerHandler);
	}
}

void AGameNetworking::GameReadyComplete()
{
	GameSocketObject->S_GameReadyComplete();
}

// ���� �õ� �Լ� 
void AGameNetworking::ProductMonsterBear(int spawnPosition, int SpawnTime)
{
	GameSocketObject->S_ProductMonsterBear(spawnPosition,SpawnTime);
}

// ���� �õ� �Լ� ���Ÿ� ĳ���� 
void AGameNetworking::ProductMonsterMurdock(int spawnPosition, int SpawnTime)
{
	GameSocketObject->S_ProductMonsterMurdock(spawnPosition, SpawnTime);
}