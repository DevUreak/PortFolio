// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MonsterBase.h"
#include "AOWGameInstance.h"
#include "BehaviorTree/BehaviorTree.h"
#include "AI_MurdockChar.generated.h"

/**
 * 
 */
UCLASS()
class AGEOFWARGAMECLIENT2_API AAI_MurdockChar : public AMonsterBase
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable, Category = "MurcoeckChar")
		void Fire(FVector Location, FRotator Rotator);
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MurcoeckChar")
		FVector BulletOffset; // 불렛 스폰위치 
	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category = "MurcoeckChar")
		TSubclassOf<class ABullet> BulletClass; // 총알 

public:
	int lastAttackGameTime; // 피격 시스템에 사용되는 변수 
	UAOWGameInstance* gameInstance;
	AAI_MurdockChar();
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AIBEAR")
		UBehaviorTree* AIBehaviorTree; // AI 전용 행동 트리 

public:
	UFUNCTION(BlueprintCallable, Category = "AIBEAR")
		void CallBP_RefreshHPBar(int nowHelath); // hpbar 변경 시키기 

public:
	void CallBP_ChagneHPBar(int EnemyType); // hp bar change 시키기 
	void InitMurdockInfo(int MonsterIndex); // 몬스터 기본정보 셋팅 

};
