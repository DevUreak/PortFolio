// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "Service_monsterHITShot.generated.h"

/**
 * 
 */
UCLASS()
class AGEOFWARGAMECLIENT2_API UService_monsterHITShot : public UBTService
{
	GENERATED_BODY()
	
public:
	UService_monsterHITShot();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
