// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameStruct.generated.h"

UCLASS()
class AGEOFWARGAMECLIENT2_API AGameStruct : public AActor
{
	GENERATED_BODY()
	

public:
	AGameStruct();

};


USTRUCT(Atomic, BlueprintType)
struct FCustomStructProduct
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GPACKET")
		int ProductEventType; // 생산 이벤트 23=생산
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GPACKET")
		int RequestUserType; // 요청자 타입 1==왼쪽 유저 2==오른쪽 유저 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GPACKET")
		int SoliderType; // 병사 종류 1=Bear, 2=?? robot?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GPACKET")
		int SpawnTime; // 스폰 시작 시간 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GPACKET")
		int SpawnPosition; // 스폰 위치 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GPACKET")
		int SpawnCompleteTime; // 스폰 완료까지 걸리는 시간 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GPACKET")
		int SpawnPosible; // 스폰 가능 여부 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GPACKET")
		bool ProCompleteFlag=false; // 생산 완료 가능한지 여부 
};

USTRUCT(Atomic, BlueprintType)
struct FCustomStructGameOption
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAMEOPTION")
		FString game_sessionid;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAMEOPTION")
		FString game_macid;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAMEOPTION")
		FString game_usernickname;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAMEOPTION")
		FString game_gameindex; // 게임 방 식별
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAMEOPTION")
		int userType; // 게임방 생성 타입 1,2 1이 생성 // 1=왼쪽 2=오른쪽

};

USTRUCT(Atomic, BlueprintType)
struct FCustomStructGPacket
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GPACKET")
		int msgType; // 게임 메세지 식별자 

};

USTRUCT(Atomic, BlueprintType)
struct FCustomStructSpawnPosition
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SPAWN")
		FVector LT_location;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SPAWN")
		FRotator LT_rotator;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SPAWN")
		FVector LM_location;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SPAWN")
		FRotator LM_rocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SPAWN")
		FVector LB_location;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SPAWN")
		FRotator LB_rocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SPAWN")
		FVector RT_location;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SPAWN")
		FRotator RT_rocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SPAWN")
		FVector RM_location;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SPAWN")
		FRotator RM_rocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SPAWN")
		FVector RB_location;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SPAWN")
		FRotator RB_rocation;

};

// 피격 이벤트 프로토콜 
USTRUCT(Atomic, BlueprintType)
struct FCustomStructHitMsg
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitMsg")
		int AttacKer; // 공격하는 MonsterIndex 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitMsg")
		int AttackGameTime; // 공격한 GameTime시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitMsg")
		int AttackDmg; // 공격 데미지 
	
};

