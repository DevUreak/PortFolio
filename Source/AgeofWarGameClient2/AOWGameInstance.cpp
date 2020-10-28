// Fill out your copyright notice in the Description page of Project Settings.


#include "AOWGameInstance.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/Networking/Public/Networking.h"
#include "HAL/RunnableThread.h"

UAOWGameInstance::UAOWGameInstance()
{
	UE_LOG(LogClass, Warning, TEXT("CLEAR GAME OPTION SETTING"));
	opperReadState = false;
	GameTime = 0;
	client_GameGold = 0;

}

FCustomStructGPacket UAOWGameInstance::getGameEventData()
{
	FCustomStructGPacket data = TEventGameData[0];
	return data;
}

int UAOWGameInstance::getGameEventHandlerSize()
{
	return TEventGameData.Num();
}

void UAOWGameInstance::removeGameEventHandlerData()
{
	TEventGameData.RemoveAt(0);
}

