// Fill out your copyright notice in the Description page of Project Settings.


#include "AOWGameInstance.h"
#include "AutoGameTime.h"

AutoGameTime::AutoGameTime(UAOWGameInstance* tgameInstance)
{
	gameInstance = tgameInstance;
	waitTimeThread = true;
	bStopThread = true;
}

uint32 AutoGameTime::Run()
{
	while (waitTimeThread){ FPlatformProcess::Sleep(0.1); }

	UE_LOG(LogClass, Warning, TEXT("TIME THREAD RUN"));

	while (bStopThread)
	{
		gameInstance->GameTime = gameInstance->GameTime + 10;
		FPlatformProcess::Sleep(0.1);
	}

	return 0;
}

void AutoGameTime::StartAutoTime() 
{
	waitTimeThread = false;
}


void AutoGameTime::Stop()
{
	// Force our thread to stop early
	bStopThread = false;
}

void AutoGameTime::Exit()
{
	// Here's where we can do any cleanup we want to 
}
