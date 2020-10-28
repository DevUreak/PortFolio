// Fill out your copyright notice in the Description page of Project Settings.


#include "Service_monsterHITShot.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AOWGameInstance.h"
#include "AI_BearController.h"
#include "AI_BearChar.h"

UService_monsterHITShot::UService_monsterHITShot() 
{
	NodeName = TEXT("ServiceMonsterHITShot");
}

void UService_monsterHITShot::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	UAOWGameInstance* gameInstance = Cast<UAOWGameInstance>(UGameplayStatics::GetGameInstance(OwnerComp.GetAIOwner()->GetWorld()));

}