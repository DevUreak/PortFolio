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
	//gameInstance = GEngine->GetWorld()->GetGameInstance<UAOWGameInstance>(); // 게임 모드에서는 됐엇음 왜냐? 월드가 존재하고 null 값이 아니였으니깐 
}

// Called when the game starts or when spawned
void AGameNetworking::BeginPlay()
{
	gameInstance = Cast<UAOWGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())); // 게임인스턴스 초기화 
}

// Called every frame
void AGameNetworking::Tick(float DeltaTime)
{

}

// 게임 소켓 스레드 run 및 연결 
void AGameNetworking::ConnGameServer()
{
	if (!CurrentThread && FPlatformProcess::SupportsMultithreading())
	{

		// 오토 타임 스레드 run
		AutoGameTimeObject = new AutoGameTime(gameInstance);
		CurrentThread2 = FRunnableThread::Create(AutoGameTimeObject, TEXT("AutoTimeThread"));

		// 게임 소켓 스레드 run 
		GameSocketObject = new GameSocket(gameInstance, AutoGameTimeObject);
		CurrentThread = FRunnableThread::Create(GameSocketObject, TEXT("SocketThread"));

		//GetWorldTimerManager().SetTimer(GameTimerHandler, this, &AGameNetworking::RunningTime, 2.0f, true, 1.0f); // 1초뒤 2초마다 이벤트 함수 실행 
		//GetWorldTimerManager().PauseTimer(GameTimerHandler);
	}
}

void AGameNetworking::GameReadyComplete()
{
	GameSocketObject->S_GameReadyComplete();
}

// 생산 시도 함수 
void AGameNetworking::ProductMonsterBear(int spawnPosition, int SpawnTime)
{
	GameSocketObject->S_ProductMonsterBear(spawnPosition,SpawnTime);
}

// 생산 시도 함수 원거리 캐릭터 
void AGameNetworking::ProductMonsterMurdock(int spawnPosition, int SpawnTime)
{
	GameSocketObject->S_ProductMonsterMurdock(spawnPosition, SpawnTime);
}