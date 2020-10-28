// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Runtime/Networking/Public/Networking.h"
#include "ThreadTaskManager.generated.h"

/**
 * 
 */
UCLASS()
class AGEOFWARGAMECLIENT2_API UThreadTaskManager : public UObject
{
	GENERATED_BODY()

public:
	UThreadTaskManager(const FObjectInitializer& ObjectInitializer);
	

public:
	void CountDownStartProcess(int* CountDownVar);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ThreadM")
		FString threadString;

public:
	class CountDownThread* MyThreadExample = nullptr;
	FRunnableThread* CurrentThread = nullptr;
	
};
