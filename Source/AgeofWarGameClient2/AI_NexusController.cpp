// Fill out your copyright notice in the Description page of Project Settings.


#include "AI_NexusController.h"
#include "AI_Nexus.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"

AAI_NexusController::AAI_NexusController()
{

	key_NowLocation= TEXT("NowLocation"); 
	key_initFlag = TEXT("NowLocation");
	BlackboardAsset = NewObject<UBlackboardData>();
	BlackboardCOMPONENT = CreateDefaultSubobject<UBlackboardComponent>(FName("BB_Nexus"));
	BehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BT_Nexus"));
	//UE_LOG(LogClass, Warning, TEXT("OK AI NEXUS!!! "));

}

void AAI_NexusController::OnPossess(class APawn* InPawn)
{
	Super::OnPossess(InPawn);
	gameInstance = Cast<UAOWGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	AAI_Nexus* AINexus = Cast<AAI_Nexus>(InPawn);
	BlackboardCOMPONENT->InitializeBlackboard(*AINexus->AIBehaviorTree->BlackboardAsset);
	BlackboardCOMPONENT->SetValueAsVector(key_NowLocation, AINexus->GetActorLocation()); //�� �ؼ��� ���� ��ġ ����ȭ 
	BlackboardCOMPONENT->SetValueAsBool(key_initFlag, true); //�� �ؼ��� �ν��Ϥ��� �ʃ��� 

	BehaviorComp->StartTree(*AINexus->AIBehaviorTree); //  �ൿ Ʈ������ 

	
}
