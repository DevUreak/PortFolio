// Fill out your copyright notice in the Description page of Project Settings.


#include "Engine/Engine.h"
#include "CountDownThread.h"


CountDownThread::CountDownThread(int* CountDownVar)
{
	this->CountDownVar = CountDownVar; // 주소 연결 
}


uint32 CountDownThread::Run()
{

	for (int i = 0; i < 10; i++)
	{
		FPlatformProcess::Sleep(1);
		*CountDownVar = *CountDownVar - 1;
	}

	return 0;
}


void CountDownThread::Exit()
{
	// Here's where we can do any cleanup we want to 
}


void CountDownThread::Stop()
{
	// Force our thread to stop early
	bStopThread = true;
}
