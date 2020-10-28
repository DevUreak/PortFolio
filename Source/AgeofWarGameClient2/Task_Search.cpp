// Fill out your copyright notice in the Description page of Project Settings.


#include "Task_Search.h"
#include "AI_BearChar.h"
#include "AI_BearController.h"
#include "AOWGameInstance.h"
#include "Math/UnrealMathUtility.h"
#include "BehaviorTree/BlackboardComponent.h"


UTask_Search::UTask_Search() 
{
	NodeName = TEXT("Task_Search"); // 노출 되는 이름 수정 
}

// Search TASK는 반드시 MOVE, ATTACK TASK 들이 완전히 끝난다음에 실행되는 구문이다, 반드시 SearchTime 동기화가 먼저 이루어져야한다.!! 
EBTNodeResult::Type UTask_Search::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	UAOWGameInstance* gameInstance = Cast<UAOWGameInstance>(UGameplayStatics::GetGameInstance(OwnerComp.GetAIOwner()->GetWorld()));

	auto monster = Cast<AMonsterBase>(OwnerComp.GetAIOwner()->GetPawn());
	FVector myMonsterLocation = monster->MonsterNowLocation; // 현재 몬스터 위치
	FVector targetMonsterLocation; //  상대방 몬스터 위치 

	float distance = 10000; // 현재 몬스터와의 거리 
	int targetMonsterIndex=0; // 타겟팅된 몬스터의 고유 식별자 // 블랙보드에 넘겨줄거임 


	if (monster->SearchGameTime <= gameInstance->GameTime) { // 서치 타임이 같거나 넘는경우 실행 됨 
		UE_LOG(LogClass, Warning, TEXT(" OK SearchTime"));
		//monster->Anim_MonsterAction = 0; // idle상태로 넘어왓!
	}
	else {
		return EBTNodeResult::Succeeded;
	}
	
	int monsterSpawnSize = gameInstance->SpawnMonsterList.Num();
	for (int i = 0; i < monsterSpawnSize; i++)
	{
		if (gameInstance->SpawnMonsterList.IsValidIndex(i) == false) //false 라면 더이상 실행 ㄴㄴ 배열 index가 없다는거니깐
		{
			UE_LOG(LogClass, Warning, TEXT(" NOT VALID"));
			continue;
		}

		if (gameInstance->SpawnMonsterList[i] == nullptr) // 죽었다는 얘기임 actor 디스트로이 함수호출되고 일정시간되서 완전히 사라진거임 
		{
			UE_LOG(LogClass, Warning, TEXT("NULL PTR DESTORY"));
			continue;
		}

		if (gameInstance->SpawnMonsterList[i]->MonsterAlive == true) // 죽은 판정을 받은상태 아직 actor 디스트로이를 호출 하지않은상태 
		{
			UE_LOG(LogClass, Warning, TEXT("DEATH MONSTER"));
			continue;
		}

		if (monster->EnemyType == gameInstance->SpawnMonsterList[i]->EnemyType) { //아군이면 다음걸 검색하자 
			UE_LOG(LogClass, Warning, TEXT("Friendly MONSTER"));
			continue;
		}

		int tmpMonsterIndex;
		// 움직이는 상태 판별하여 타겟 위치 초기화 , tmp가 있는이유는 혹시나 null ptr 떄문에 참조하지못할까봐.. ㅠ -> 이거 방지하는 방법은 기존에 있던 list를 복사해서 사용하는 방법이있음
		if (gameInstance->SpawnMonsterList[i]->MonsterMoving == true) {
			targetMonsterLocation = gameInstance->SpawnMonsterList[i]->MonsterMovingLocaiton;
			tmpMonsterIndex = gameInstance->SpawnMonsterList[i]->monsterIndex;
		}
		else
			targetMonsterLocation = gameInstance->SpawnMonsterList[i]->MonsterNowLocation; {
			tmpMonsterIndex = gameInstance->SpawnMonsterList[i]->monsterIndex;
		}

		// 지금 몬스터와의 타겟의 디스턴스 
		float preDistance = monster->SearchforTarget(myMonsterLocation, targetMonsterLocation);
		preDistance = FMath::RoundFromZero(preDistance);
		
		//이전 거리와 지금과 비교 한다면?
		if(preDistance < distance)
		{
			distance = preDistance;
			targetMonsterIndex = tmpMonsterIndex;
		}

		UE_LOG(LogClass, Warning, TEXT(" MonsterInfo Mindex -> %d, distance -> %f  "), targetMonsterIndex,distance);
	}


	if (distance < monster->MonsterRange) { //true == 공격 action false == move aciton
		OwnerComp.GetBlackboardComponent()->SetValueAsInt(FName("MonsterActionType"), 1); 
		OwnerComp.GetBlackboardComponent()->SetValueAsInt(FName("TargetIndex"), targetMonsterIndex);
		UE_LOG(LogClass, Warning, TEXT(" target attaking index -> %d, MonsterRange -> %f  "), targetMonsterIndex, monster->MonsterRange);

	}
	else {
		OwnerComp.GetBlackboardComponent()->SetValueAsInt(FName("MonsterActionType"), 2);
		OwnerComp.GetBlackboardComponent()->SetValueAsInt(FName("TargetIndex"), targetMonsterIndex);
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(FName("TargetLocation"), targetMonsterLocation);
		OwnerComp.GetBlackboardComponent()->SetValueAsFloat(FName("TargetMonsterDistance"), distance);
	}
			
	return EBTNodeResult::Failed;
}