// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HAL/Runnable.h"
#include "CoreMinimal.h"

/**
 * 
 */
class AGEOFWARGAMECLIENT2_API CountDownThread : public FRunnable
{

public:
	CountDownThread(int* InTargetCount);

	int* CountDownVar;
	// FRunnable functions
	virtual uint32 Run() override;
	virtual void Stop() override;
	virtual void Exit() override;
	// FRunnable


protected:
	bool bStopThread = false;

};
