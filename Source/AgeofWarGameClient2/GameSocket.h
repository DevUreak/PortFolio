// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HAL/Runnable.h"
#include "GameStruct.h"
#include "AutoGameTime.h"
#include "Runtime/Networking/Public/Networking.h"
#include "AOWGameInstance.h"
#include "Dom/JsonObject.h"
#include "CoreMinimal.h"

/**
 * 
 */
class AGEOFWARGAMECLIENT2_API GameSocket : public FRunnable
{

public:
	GameSocket(UAOWGameInstance* gameInstance, class AutoGameTime* tautotimer);
	virtual uint32 Run() override;
	virtual void Stop() override;
	virtual void Exit() override;


public:
	bool ThreadRuning;
	UAOWGameInstance* gameInstance;
	FSocket* ConnGameSocket;
	class AutoGameTime* autotimer;

public:
	FString StringFromBinaryArray(TArray<uint8> BinaryArray);
	void recvMatchEventHandle(TSharedPtr<FJsonObject> JsonObject);
	bool SendMessage(const FString& Message);
	void BinaryArrayFromString(const FString& Message, TArray<uint8>& OutBinaryArray); 
	bool Parse(const FString& MaskString, FIPv4Address& OutMask);
	FString getGameBaseData();
	bool GameConnCreateSocket(const FString& YourChosenSocketName, const FString& TheIP, const int32 ThePort);
	void ActiveGame(TSharedPtr<FJsonObject> JsonObject, FCustomStructGPacket* packet); //게임 시작 
	void OpperReadyConfirm();
	void S_GameReadyComplete();
	void S_ProductMonsterBear(int spawnPosition, int SpawnTime); //Bear 생산 이벤트 전송 
	void SetProductMsg(TSharedPtr<FJsonObject> JsonObject); // 생산시스템으로 전송 
	void S_ProductMonsterMurdock(int spawnPosition,int SpawnTime); //머독 생산 이벤트 전송

};
