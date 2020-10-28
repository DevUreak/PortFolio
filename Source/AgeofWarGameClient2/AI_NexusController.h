// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BehaviorTree.h"
#include "AOWGameInstance.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI_NexusController.generated.h"

/**
 * 
 */
UCLASS()
class AGEOFWARGAMECLIENT2_API AAI_NexusController : public AAIController
{
	GENERATED_BODY()

public:
	AAI_NexusController();
	virtual void OnPossess(class APawn* InPawn) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AICONBEAR")
		int con_MonsterIndex; // ���� �����ĺ���  
	UPROPERTY(transient)
		UBlackboardComponent* BlackboardCOMPONENT; //������ ������Ʈ
	UPROPERTY(transient)
		UBehaviorTreeComponent* BehaviorComp; // �ൿƮ�� ������Ʈ 

public:
	FName key_NowLocation;
	FName key_initFlag;
	UBlackboardData* BlackboardAsset;
	UAOWGameInstance* gameInstance;
};
