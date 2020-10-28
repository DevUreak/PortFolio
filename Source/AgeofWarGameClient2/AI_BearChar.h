// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MonsterBase.h"
#include "AOWGameInstance.h"
#include "BehaviorTree/BehaviorTree.h"
#include "AI_BearChar.generated.h"

/**
 * 
 */
UCLASS()
class AGEOFWARGAMECLIENT2_API AAI_BearChar : public AMonsterBase
{
	GENERATED_BODY()
	
public:
	AAI_BearChar();
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

public:
	int lastAttackGameTime; // �ǰ� �ý��ۿ� ���Ǵ� ���� 
	UAOWGameInstance* gameInstance;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AIBEAR")
		UBehaviorTree* AIBehaviorTree; // AI ���� �ൿ Ʈ�� 

public:
	UFUNCTION(BlueprintCallable, Category = "AIBEAR")
		void CallBP_RefreshHPBar(int nowHelath); // hpbar ���� ��Ű�� 

public:
	void CallBP_ChagneHPBar(int EnemyType); // hp bar change ��Ű�� 
	void InitBearInfo(int MonsterIndex); // ���� �⺻���� ���� 

};
