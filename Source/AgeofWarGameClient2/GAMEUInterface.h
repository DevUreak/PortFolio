// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Dom/JsonObject.h"
#include "AI_BearChar.h"
#include "GAMEUInterface.generated.h"

/**
 * 
 */
UCLASS()
class AGEOFWARGAMECLIENT2_API UGAMEUInterface : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "gameui")
		AAI_BearChar* SpawnMonster(int EnmeyType, FVector Loc);

	//FString CreateMsg_solider(int soilderType, int orderPlayerIndex);
};
