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
		int ProductEventType; // ���� �̺�Ʈ 23=����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GPACKET")
		int RequestUserType; // ��û�� Ÿ�� 1==���� ���� 2==������ ���� 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GPACKET")
		int SoliderType; // ���� ���� 1=Bear, 2=?? robot?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GPACKET")
		int SpawnTime; // ���� ���� �ð� 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GPACKET")
		int SpawnPosition; // ���� ��ġ 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GPACKET")
		int SpawnCompleteTime; // ���� �Ϸ���� �ɸ��� �ð� 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GPACKET")
		int SpawnPosible; // ���� ���� ���� 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GPACKET")
		bool ProCompleteFlag=false; // ���� �Ϸ� �������� ���� 
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
		FString game_gameindex; // ���� �� �ĺ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAMEOPTION")
		int userType; // ���ӹ� ���� Ÿ�� 1,2 1�� ���� // 1=���� 2=������

};

USTRUCT(Atomic, BlueprintType)
struct FCustomStructGPacket
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GPACKET")
		int msgType; // ���� �޼��� �ĺ��� 

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

// �ǰ� �̺�Ʈ �������� 
USTRUCT(Atomic, BlueprintType)
struct FCustomStructHitMsg
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitMsg")
		int AttacKer; // �����ϴ� MonsterIndex 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitMsg")
		int AttackGameTime; // ������ GameTime�ð�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitMsg")
		int AttackDmg; // ���� ������ 
	
};

