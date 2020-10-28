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
	BlackboardCOMPONENT->SetValueAsVector(key_NowLocation, AINexus->GetActorLocation()); //각 넥서스 현재 위치 동기화 
	BlackboardCOMPONENT->SetValueAsBool(key_initFlag, true); //각 넥서스 인스턴ㄴ스 초깋허 

	BehaviorComp->StartTree(*AINexus->AIBehaviorTree); //  행동 트리시작 

	
}
