// Fill out your copyright notice in the Description page of Project Settings.


#include "AI_BearController.h"
#include "AI_BearChar.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"


const FName AAI_BearController::key_InitFlag = TEXT("initFlag");
const FName AAI_BearController::key_MonsterActiontype = TEXT("MonsterActionType");

AAI_BearController::AAI_BearController() 
{
	
	BlackboardAsset = NewObject<UBlackboardData>();
	BlackboardCOMPONENT = CreateDefaultSubobject<UBlackboardComponent>(FName("BB_Bear"));
	BehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BT_Bear"));

	/*BlackboardAsset->UpdatePersistentKey<UBlackboardKeyType_Object>(FName("KeyMap"));
	BlackboardCOMPONENT->InitializeBlackboard(*BlackboardAsset);*/ // BT없이 인스턴스 초기화 할때 사용 OnUnPossess 
}

void AAI_BearController::OnPossess(class APawn* InPawn)
{
	Super::OnPossess(InPawn);
	gameInstance = Cast<UAOWGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	AAI_BearChar* AIBearCharact = Cast<AAI_BearChar>(InPawn);
	
	BlackboardCOMPONENT->InitializeBlackboard(*AIBearCharact->AIBehaviorTree->BlackboardAsset);
	BlackboardCOMPONENT->SetValueAsBool(key_InitFlag, true); // 스폰이후 BT 첫 실행시 INIT TASK를 한번만 실행시키기위한 FLAG임 
	BehaviorComp->StartTree(*AIBearCharact->AIBehaviorTree); //  행동 트리시작 

}

void AAI_BearController::OnUnPossess()
{
	Super::OnUnPossess();

	UE_LOG(LogClass, Warning, TEXT("unpossess 1!! ai con !!!!!!!"));


}