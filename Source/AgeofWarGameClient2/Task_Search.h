// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Task_Search.generated.h"

/**
 * 
 */
UCLASS()
class AGEOFWARGAMECLIENT2_API UTask_Search : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UTask_Search();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) override;

	

};
