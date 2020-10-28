// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ThreadTaskManager.h"
#include "GameStruct.h"
#include "MonsterBase.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BehaviorTreeComponent.h"
#include "Runtime/Networking/Public/Networking.h"
#include "HAL/RunnableThread.h"
#include "AOWGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class AGEOFWARGAMECLIENT2_API UAOWGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UAOWGameInstance();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAMEINSTANCE")
		TArray<FCustomStructProduct> Product_GPacket; // 생산 시스템에서 사용하는 패킷 모음 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAMEINSTANCE")
		TArray<FCustomStructProduct> ProductUIGPacket; // 생산 시스템중 ui 시스템에서ㅗ 사용하는 변수 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAMEINSTANCE")
		FCustomStructGameOption fsGameOption; // 로그인 레벨 옵션들 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAMEINSTANCE")
		int GameTime; //현재 게임  시간 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAMEINSTANCE")
		TArray<FCustomStructGPacket> TEventGameData; // recv game 데이터 정리
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAMEINSTANCE")
		bool opperReadState; // 상대방 살이있는지 확인 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAMEINSTANCE")
		int client_GameGold; // 현재 게임 골드 상태  
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAMEINSTANCE")
		int client_AutoGold; // 오토 골드 상태
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAMEINSTANCE")
		FTimerHandle GameEventTimerHandle; //게임 이벤트 타이머 변수 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAMEINSTANCE")
		TArray<AMonsterBase*> SpawnMonsterList; //생산이후 스폰된 몬스터 리스트 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAMEINSTANCE")
		bool GameResultFlag; //  게임이 끝났는지 체크 //  게임 시작시 반드시 초기화시켜줘야함
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAMEINSTANCE")
		int GameResultUser; // 어떤 플레이어가 이겼는지 1== 왼 2== 오  //  게임 시작시 반드시 초기화시켜줘야함
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAMEINSTANCE")
		UAudioComponent *BackGroundMusic; // 백그라운드 뮤직!

public: 
	UFUNCTION(BlueprintCallable, Category = "GAMEINSTANCE")
		FCustomStructGPacket getGameEventData(); // 게임 서버 연결및 초기화 작업 
	UFUNCTION(BlueprintCallable, Category = "GAMEINSTANCE")
		int getGameEventHandlerSize(); // 게임 서버 연결및 초기화 작업 
	UFUNCTION(BlueprintCallable, Category = "GAMEINSTANCE")
		void removeGameEventHandlerData(); // 게임 서버 연결및 초기화 작업 


};