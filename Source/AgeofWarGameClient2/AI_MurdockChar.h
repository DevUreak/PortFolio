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
		FVector BulletOffset; // �ҷ� ������ġ 
	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category = "MurcoeckChar")
		TSubclassOf<class ABullet> BulletClass; // �Ѿ� 

public:
	int lastAttackGameTime; // �ǰ� �ý��ۿ� ���Ǵ� ���� 
	UAOWGameInstance* gameInstance;
	AAI_MurdockChar();
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AIBEAR")
		UBehaviorTree* AIBehaviorTree; // AI ���� �ൿ Ʈ�� 

public:
	UFUNCTION(BlueprintCallable, Category = "AIBEAR")
		void CallBP_RefreshHPBar(int nowHelath); // hpbar ���� ��Ű�� 

public:
	void CallBP_ChagneHPBar(int EnemyType); // hp bar change ��Ű�� 
	void InitMurdockInfo(int MonsterIndex); // ���� �⺻���� ���� 

};
