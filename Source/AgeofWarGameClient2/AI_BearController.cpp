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
	BlackboardCOMPONENT->InitializeBlackboard(*BlackboardAsset);*/ // BT���� �ν��Ͻ� �ʱ�ȭ �Ҷ� ��� OnUnPossess 
}

void AAI_BearController::OnPossess(class APawn* InPawn)
{
	Super::OnPossess(InPawn);
	gameInstance = Cast<UAOWGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	AAI_BearChar* AIBearCharact = Cast<AAI_BearChar>(InPawn);
	
	BlackboardCOMPONENT->InitializeBlackboard(*AIBearCharact->AIBehaviorTree->BlackboardAsset);
	BlackboardCOMPONENT->SetValueAsBool(key_InitFlag, true); // �������� BT ù ����� INIT TASK�� �ѹ��� �����Ű������ FLAG�� 
	BehaviorComp->StartTree(*AIBearCharact->AIBehaviorTree); //  �ൿ Ʈ������ 

}

void AAI_BearController::OnUnPossess()
{
	Super::OnUnPossess();

	UE_LOG(LogClass, Warning, TEXT("unpossess 1!! ai con !!!!!!!"));


}