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
	bool tickDeathFlag; // tick �̺�Ʈ���� ����ó���� ���Ǵ� ����  

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterBase")
		int EnemyType; // 1���� ���� 2 ������ ���� 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterBase")
		int MonsterType; // 0�ؼ��� 1bear 2robot
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterBase")
		int monsterIndex; //���� ���� �ĺ��� 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterBase")
		int startSpawnGameTime; //���� ���� gameTime
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterBase")
		int MoveGameTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterBase")
		int SearchGameTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterBase")
		bool MonsterAlive; //����ִ°�..? 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterBase")
		int MAXMonsterHelath; // MAX ü�� 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterBase")
		int MonsterHelath; // ���� ü�� 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterBase")
		int MonsterDamage; // ���ݷ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterBase")
		float MonsterRange; // ��Ÿ� 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterBase")
		int Anim_MonsterAction; // ���� �׼� ���� == 1 ����  

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterBase")
		bool MonsterMoving; //�����̴����ΰ�? true == ������, false == dont������ 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterBase")
		FVector MonsterNowLocation; // ���� ��ġ 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterBase")
		FVector MonsterMovingLocaiton; // �̷�(�̵��ϰ��ִ� ����) ��ġ 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterBase")
		TArray<FCustomStructHitMsg> HitEventList; //�ǰݵ� �̺�Ʈ���� �ִ°�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterBase")
		bool MonsterATTAKING; // ���Ͱ� ���������� ��Ÿ�� TRUE ������ 



public:
	UFUNCTION(BlueprintCallable, Category = "MonsterLogic")
		float SearchforTarget(FVector myLocation, FVector targetLocation);
	UFUNCTION(BlueprintCallable, Category = "MonsterLogic")
		void MonsterDeathDestory(); // ���� ����ó���� ��Ʈ���� 
	UFUNCTION(BlueprintCallable, Category = "MonsterLogic")
		void MonsterDeathDestoryFinal(); // �������� 
};
