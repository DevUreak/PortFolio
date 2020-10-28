// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MonsterBase.h"
#include "AOWGameInstance.h"
#include "BehaviorTree/BehaviorTree.h"
#include "AI_Nexus.generated.h"

UCLASS()
class AGEOFWARGAMECLIENT2_API AAI_Nexus : public AMonsterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAI_Nexus();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AIBEAR")
		UBehaviorTree* AIBehaviorTree; // AI 전용 행동 트리 

public:	
	void CallBP_RefreshHPBar(int nowHelath);

public:
	int lastAttackGameTime;
	UAOWGameInstance* gameInstance;

};
