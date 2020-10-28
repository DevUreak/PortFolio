// Fill out your copyright notice in the Description page of Project Settings.


#include "ThreadTaskManager.h"
#include "CountDownThread.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/Networking/Public/Networking.h"
#include "HAL/RunnableThread.h"

UThreadTaskManager::UThreadTaskManager(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	threadString = "thread string run";
}

void UThreadTaskManager::CountDownStartProcess(int* CountDownVar)
{
	if (!CurrentThread && FPlatformProcess::SupportsMultithreading())
	{
		// Run the thread until we've found 999 random numbers
		MyThreadExample = new CountDownThread(CountDownVar);
		CurrentThread = FRunnableThread::Create(MyThreadExample, TEXT("Any old thread name"));
	}
}
