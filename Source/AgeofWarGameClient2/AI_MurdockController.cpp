// Fill out your copyright notice in the Description page of Project Settings.


#include "AI_MurdockController.h"
#include "AI_MurdockChar.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"


const FName AAI_MurdockController::key_InitFlag2 = TEXT("initFlag");
const FName AAI_MurdockController::key_MonsterActiontype2 = TEXT("MonsterActionType");

AAI_MurdockController::AAI_MurdockController()
{

	BlackboardAsset = NewObject<UBlackboardData>();
	BlackboardCOMPONENT = CreateDefaultSubobject<UBlackboardComponent>(FName("BB_Murdock"));
	BehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BT_Murdock"));

	/*BlackboardAsset->UpdatePersistentKey<UBlackboardKeyType_Object>(FName("KeyMap"));
	BlackboardCOMPONENT->InitializeBlackboard(*BlackboardAsset);*/ // BT���� �ν��Ͻ� �ʱ�ȭ �Ҷ� ��� OnUnPossess 
}

void AAI_MurdockController::OnPossess(class APawn* InPawn)
{
	Super::OnPossess(InPawn);
	gameInstance = Cast<UAOWGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	AAI_MurdockChar* AIMurdockCharact = Cast<AAI_MurdockChar>(InPawn);

	BlackboardCOMPONENT->InitializeBlackboard(*AIMurdockCharact->AIBehaviorTree->BlackboardAsset);
	BlackboardCOMPONENT->SetValueAsBool(key_InitFlag2, true); // �������� BT ù ����� INIT TASK�� �ѹ��� �����Ű������ FLAG�� 

	BehaviorComp->StartTree(*AIMurdockCharact->AIBehaviorTree); //  �ൿ Ʈ������ 

}

void AAI_MurdockController::OnUnPossess()
{
	Super::OnUnPossess();

	UE_LOG(LogClass, Warning, TEXT("unpossess 1!! ai con !!!!!!!"));


}