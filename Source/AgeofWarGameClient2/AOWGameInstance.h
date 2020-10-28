// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ThreadTaskManager.h"
#include "GameStruct.h"
#include "MonsterBase.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BehaviorTreeComponent.h"
#include "Runtime/Networking/Public/Networking.h"
#include "HAL/RunnableThread.h"
#include "AOWGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class AGEOFWARGAMECLIENT2_API UAOWGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UAOWGameInstance();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAMEINSTANCE")
		TArray<FCustomStructProduct> Product_GPacket; // ���� �ý��ۿ��� ����ϴ� ��Ŷ ���� 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAMEINSTANCE")
		TArray<FCustomStructProduct> ProductUIGPacket; // ���� �ý����� ui �ý��ۿ����� ����ϴ� ���� 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAMEINSTANCE")
		FCustomStructGameOption fsGameOption; // �α��� ���� �ɼǵ� 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAMEINSTANCE")
		int GameTime; //���� ����  �ð� 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAMEINSTANCE")
		TArray<FCustomStructGPacket> TEventGameData; // recv game ������ ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAMEINSTANCE")
		bool opperReadState; // ���� �����ִ��� Ȯ�� 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAMEINSTANCE")
		int client_GameGold; // ���� ���� ��� ����  
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAMEINSTANCE")
		int client_AutoGold; // ���� ��� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAMEINSTANCE")
		FTimerHandle GameEventTimerHandle; //���� �̺�Ʈ Ÿ�̸� ���� 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAMEINSTANCE")
		TArray<AMonsterBase*> SpawnMonsterList; //�������� ������ ���� ����Ʈ 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAMEINSTANCE")
		bool GameResultFlag; //  ������ �������� üũ //  ���� ���۽� �ݵ�� �ʱ�ȭ���������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAMEINSTANCE")
		int GameResultUser; // � �÷��̾ �̰���� 1== �� 2== ��  //  ���� ���۽� �ݵ�� �ʱ�ȭ���������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAMEINSTANCE")
		UAudioComponent *BackGroundMusic; // ��׶��� ����!

public: 
	UFUNCTION(BlueprintCallable, Category = "GAMEINSTANCE")
		FCustomStructGPacket getGameEventData(); // ���� ���� ����� �ʱ�ȭ �۾� 
	UFUNCTION(BlueprintCallable, Category = "GAMEINSTANCE")
		int getGameEventHandlerSize(); // ���� ���� ����� �ʱ�ȭ �۾� 
	UFUNCTION(BlueprintCallable, Category = "GAMEINSTANCE")
		void removeGameEventHandlerData(); // ���� ���� ����� �ʱ�ȭ �۾� 


};