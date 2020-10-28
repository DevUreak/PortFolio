// Fill out your copyright notice in the Description page of Project Settings.


#include "Task_Search.h"
#include "AI_BearChar.h"
#include "AI_BearController.h"
#include "AOWGameInstance.h"
#include "Math/UnrealMathUtility.h"
#include "BehaviorTree/BlackboardComponent.h"


UTask_Search::UTask_Search() 
{
	NodeName = TEXT("Task_Search"); // ���� �Ǵ� �̸� ���� 
}

// Search TASK�� �ݵ�� MOVE, ATTACK TASK ���� ������ ���������� ����Ǵ� �����̴�, �ݵ�� SearchTime ����ȭ�� ���� �̷�������Ѵ�.!! 
EBTNodeResult::Type UTask_Search::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	UAOWGameInstance* gameInstance = Cast<UAOWGameInstance>(UGameplayStatics::GetGameInstance(OwnerComp.GetAIOwner()->GetWorld()));

	auto monster = Cast<AMonsterBase>(OwnerComp.GetAIOwner()->GetPawn());
	FVector myMonsterLocation = monster->MonsterNowLocation; // ���� ���� ��ġ
	FVector targetMonsterLocation; //  ���� ���� ��ġ 

	float distance = 10000; // ���� ���Ϳ��� �Ÿ� 
	int targetMonsterIndex=0; // Ÿ���õ� ������ ���� �ĺ��� // �����忡 �Ѱ��ٰ��� 


	if (monster->SearchGameTime <= gameInstance->GameTime) { // ��ġ Ÿ���� ���ų� �Ѵ°�� ���� �� 
		UE_LOG(LogClass, Warning, TEXT(" OK SearchTime"));
		//monster->Anim_MonsterAction = 0; // idle���·� �Ѿ��!
	}
	else {
		return EBTNodeResult::Succeeded;
	}
	
	int monsterSpawnSize = gameInstance->SpawnMonsterList.Num();
	for (int i = 0; i < monsterSpawnSize; i++)
	{
		if (gameInstance->SpawnMonsterList.IsValidIndex(i) == false) //false ��� ���̻� ���� ���� �迭 index�� ���ٴ°Ŵϱ�
		{
			UE_LOG(LogClass, Warning, TEXT(" NOT VALID"));
			continue;
		}

		if (gameInstance->SpawnMonsterList[i] == nullptr) // �׾��ٴ� ����� actor ��Ʈ���� �Լ�ȣ��ǰ� �����ð��Ǽ� ������ ��������� 
		{
			UE_LOG(LogClass, Warning, TEXT("NULL PTR DESTORY"));
			continue;
		}

		if (gameInstance->SpawnMonsterList[i]->MonsterAlive == true) // ���� ������ �������� ���� actor ��Ʈ���̸� ȣ�� ������������ 
		{
			UE_LOG(LogClass, Warning, TEXT("DEATH MONSTER"));
			continue;
		}

		if (monster->EnemyType == gameInstance->SpawnMonsterList[i]->EnemyType) { //�Ʊ��̸� ������ �˻����� 
			UE_LOG(LogClass, Warning, TEXT("Friendly MONSTER"));
			continue;
		}

		int tmpMonsterIndex;
		// �����̴� ���� �Ǻ��Ͽ� Ÿ�� ��ġ �ʱ�ȭ , tmp�� �ִ������� Ȥ�ó� null ptr ������ �����������ұ��.. �� -> �̰� �����ϴ� ����� ������ �ִ� list�� �����ؼ� ����ϴ� ���������
		if (gameInstance->SpawnMonsterList[i]->MonsterMoving == true) {
			targetMonsterLocation = gameInstance->SpawnMonsterList[i]->MonsterMovingLocaiton;
			tmpMonsterIndex = gameInstance->SpawnMonsterList[i]->monsterIndex;
		}
		else
			targetMonsterLocation = gameInstance->SpawnMonsterList[i]->MonsterNowLocation; {
			tmpMonsterIndex = gameInstance->SpawnMonsterList[i]->monsterIndex;
		}

		// ���� ���Ϳ��� Ÿ���� ���Ͻ� 
		float preDistance = monster->SearchforTarget(myMonsterLocation, targetMonsterLocation);
		preDistance = FMath::RoundFromZero(preDistance);
		
		//���� �Ÿ��� ���ݰ� �� �Ѵٸ�?
		if(preDistance < distance)
		{
			distance = preDistance;
			targetMonsterIndex = tmpMonsterIndex;
		}

		UE_LOG(LogClass, Warning, TEXT(" MonsterInfo Mindex -> %d, distance -> %f  "), targetMonsterIndex,distance);
	}


	if (distance < monster->MonsterRange) { //true == ���� action false == move aciton
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