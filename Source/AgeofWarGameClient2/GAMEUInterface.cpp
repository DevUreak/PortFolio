// Fill out your copyright notice in the Description page of Project Settings.


#include "GAMEUInterface.h"
#include "Engine/Engine.h"
#include "Engine/World.h"


AAI_BearChar* UGAMEUInterface::SpawnMonster(int EnmeyType, FVector Loc) // 병사 생산 메소스 
{
	UE_LOG(LogClass, Warning, TEXT("Request Solider Production"));

	FName Path = TEXT("Blueprint'/Game/GameSystem/AI/AIBear/AI_BearChar_BP.AI_BearChar_BP_C'");
	UClass* GeneratedBP = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), NULL, *Path.ToString()));
	AAI_BearChar* mob = GetWorld()->SpawnActor<AAI_BearChar>(GeneratedBP, Loc, FRotator::ZeroRotator);

	return mob;
}
