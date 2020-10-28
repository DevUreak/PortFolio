// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BehaviorTree.h"
#include "AOWGameInstance.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI_BearController.generated.h"

/**
 * 
 */

UCLASS()
class AGEOFWARGAMECLIENT2_API AAI_BearController : public AAIController
{
	GENERATED_BODY()

	static const FName key_InitFlag;
	static const FName key_MonsterActiontype;
	
public:
	AAI_BearController();
	virtual void OnPossess(class APawn* InPawn) override;
	virtual void OnUnPossess() override;
	

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AICONBEAR")
		int con_MonsterIndex; // 행동트리 컴포넌트 
	UPROPERTY(transient)
		UBlackboardComponent* BlackboardCOMPONENT; //블랙보드 컴포넌트
	UPROPERTY(transient)
		UBehaviorTreeComponent* BehaviorComp; // 행동트리 컴포넌트 


public:
	UBlackboardData* BlackboardAsset;
	UAOWGameInstance* gameInstance;
};
