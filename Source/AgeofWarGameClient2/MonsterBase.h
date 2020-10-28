// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameStruct.h"
#include "MonsterBase.generated.h"

UCLASS()
class AGEOFWARGAMECLIENT2_API AMonsterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMonsterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


public:
	bool tickDeathFlag; // tick 이벤트에서 데스처리에 사용되는 변수  

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterBase")
		int EnemyType; // 1왼쪽 진영 2 오른쪽 진영 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterBase")
		int MonsterType; // 0넥서스 1bear 2robot
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterBase")
		int monsterIndex; //몬스터 고유 식별자 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterBase")
		int startSpawnGameTime; //몬스터 스폰 gameTime
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterBase")
		int MoveGameTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterBase")
		int SearchGameTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterBase")
		bool MonsterAlive; //살아있는가..? 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterBase")
		int MAXMonsterHelath; // MAX 체력 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterBase")
		int MonsterHelath; // 현재 체력 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterBase")
		int MonsterDamage; // 공격력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterBase")
		float MonsterRange; // 사거리 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterBase")
		int Anim_MonsterAction; // 몬스터 액션 상태 == 1 공격  

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterBase")
		bool MonsterMoving; //움직이는중인가? true == 움직임, false == dont움직임 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterBase")
		FVector MonsterNowLocation; // 현재 위치 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterBase")
		FVector MonsterMovingLocaiton; // 미래(이동하고있는 목적) 위치 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterBase")
		TArray<FCustomStructHitMsg> HitEventList; //피격된 이벤트들이 있는곳
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterBase")
		bool MonsterATTAKING; // 몬스터가 공격중인지 나타냄 TRUE 공격중 



public:
	UFUNCTION(BlueprintCallable, Category = "MonsterLogic")
		float SearchforTarget(FVector myLocation, FVector targetLocation);
	UFUNCTION(BlueprintCallable, Category = "MonsterLogic")
		void MonsterDeathDestory(); // 몬스터 데스처리및 디스트로이 
	UFUNCTION(BlueprintCallable, Category = "MonsterLogic")
		void MonsterDeathDestoryFinal(); // 최종죽음 
};
