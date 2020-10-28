// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HAL/Runnable.h"
#include "AOWGameInstance.h"
#include "CoreMinimal.h"

/**
 * 
 */
class AGEOFWARGAMECLIENT2_API AutoGameTime : public FRunnable
{
public:
	AutoGameTime(UAOWGameInstance* gameInstance);

public:
	UAOWGameInstance* gameInstance;
	bool waitTimeThread;


public:
	virtual uint32 Run() override;
	virtual void Stop() override;
	virtual void Exit() override;
	void StartAutoTime();

protected:
	bool bStopThread = true;


};
