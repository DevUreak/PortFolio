// Fill out your copyright notice in the Description page of Project Settings.


#include "ProductSYSTEM.h"
#include "Kismet/GameplayStatics.h"
#include "AI_BearChar.h"
#include "AI_Nexus.h"
#include "AI_MurdockChar.h"
#include "MonsterBase.h"
#include "Engine/Engine.h"

// Sets default values
AProductSYSTEM::AProductSYSTEM()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AProductSYSTEM::BeginPlay()
{
	Super::BeginPlay();
	monsterIndex = 3; // 0,1 은 넥서스 
	gameInstance = Cast<UAOWGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	gameInstance->GameResultFlag = false;
	gameInstance->GameResultUser = 0;

	SoldierSpawnPosition.LT_location.Set(1240.0, -540.0, 98.0); 
	SoldierSpawnPosition.LM_location.Set(2040.0, -540.0, 98.0); 
	SoldierSpawnPosition.LB_location.Set(2780.0, -540.0, 98.0); 

	SoldierSpawnPosition.RT_location.Set(1240.0, -6730.0, 98.0);
	SoldierSpawnPosition.RM_location.Set(2040.0, -6730.0, 98.0); 
	SoldierSpawnPosition.RB_location.Set(2780.0, -6730.0, 98.0);
}

// Called every frame
void AProductSYSTEM::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime); 

	if ((gameInstance->Product_GPacket.Num()!=0) && (gameInstance->Product_GPacket[0].SpawnCompleteTime == gameInstance->GameTime)) 
	{
		
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Profuct solider")));
		
		SpawnSolider(gameInstance->Product_GPacket[0].SoliderType, 
			gameInstance->Product_GPacket[0].SpawnPosition,gameInstance->Product_GPacket[0].RequestUserType,gameInstance->Product_GPacket[0].SpawnCompleteTime); //몬스터 스폰 

		if (gameInstance->Product_GPacket[0].RequestUserType == gameInstance->fsGameOption.userType) { // UI COMPLETE 셋팅 
			gameInstance->ProductUIGPacket[0].ProCompleteFlag = true;
		}

		gameInstance->Product_GPacket.RemoveAt(0);
	}
}


void AProductSYSTEM::SpawnSolider(int soliderType, int spawnPosition, int enemyType, int GameTime)
{
	//enemyType usertype 1 이면 왼쪽 , 2면 오른쪽이야 
	FVector spawnLocation;
	FName Path;
	UE_LOG(LogClass, Warning, TEXT("SPAWN MONSTER INFO solider type -> %d , spawnposition -> %d, enmyType -> %d "), soliderType, spawnPosition, enemyType);
	if (soliderType==1) {
		Path = TEXT("Blueprint'/Game/GameSystem/AI/AIBear2/AI_BearChar_BP.AI_BearChar_BP_C'");
	}
	else if (soliderType==2) {
		Path = TEXT("Blueprint'/Game/GameSystem/AI/AIMurdock/AI_MurdockChar_BP.AI_MurdockChar_BP_C'");
	}

	UClass* GeneratedBP = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), NULL, *Path.ToString()));
	if (enemyType == 1) {
		switch (spawnPosition)
		{
		case 1: // 탑
			spawnLocation = SoldierSpawnPosition.LT_location;
			break;
		case 2: //미드 
			spawnLocation = SoldierSpawnPosition.LM_location;
			break;
		case 3: //바텀
			spawnLocation = SoldierSpawnPosition.LB_location;
			break;
		}
	}
	else {
		switch (spawnPosition)
		{
		case 1: 
			spawnLocation = SoldierSpawnPosition.RT_location;
			break;
		case 2: 
			spawnLocation = SoldierSpawnPosition.RM_location;
			break;
		case 3: 
			spawnLocation = SoldierSpawnPosition.RB_location;
			break;
		}
	}

	// MonsterInit
	int mtmp = monsterIndex;
	if (soliderType==1) {
		AAI_BearChar *mob = GetWorld()->SpawnActor<AAI_BearChar>(GeneratedBP, spawnLocation, FRotator::ZeroRotator);
		mob->monsterIndex = monsterIndex; // 고유 식별자 
		mob->MonsterNowLocation = spawnLocation; // 현재 위치 
		mob->MonsterMoving = false; // 움직이지 않는상태 
		mob->EnemyType = enemyType; // 아군인지 적군인지 
		mob->startSpawnGameTime = GameTime; // 아직 사용안하는중 
		mob->SearchGameTime = GameTime + 100; // 첫번째 search task 시작 타임 

		AMonsterBase *mobase = mob;
		monsterIndex = monsterIndex + 1;
		gameInstance->SpawnMonsterList.Emplace(mobase);

		mob->InitBearInfo(mtmp); // 곰새끼 초기화 
		mob->CallBP_ChagneHPBar(enemyType); // 스폰시 hpbar 색상 변경 시키기 
	}
	else if (soliderType==2) {
		AAI_MurdockChar *mob = GetWorld()->SpawnActor<AAI_MurdockChar>(GeneratedBP, spawnLocation, FRotator::ZeroRotator);
		mob->monsterIndex = monsterIndex; // 고유 식별자 
		mob->MonsterNowLocation = spawnLocation; // 현재 위치 
		mob->MonsterMoving = false; // 움직이지 않는상태 
		mob->EnemyType = enemyType; // 아군인지 적군인지 
		mob->startSpawnGameTime = GameTime; // 아직 사용안하는중 
		mob->SearchGameTime = GameTime + 100; // 첫번째 search task 시작 타임 

		AMonsterBase *mobase = mob;
		monsterIndex = monsterIndex + 1;
		gameInstance->SpawnMonsterList.Emplace(mobase);

		mob->InitMurdockInfo(mtmp);
		mob->CallBP_ChagneHPBar(enemyType);
	}

	UE_LOG(LogClass, Warning, TEXT("INIT Monster"));
}

//스폰 넥서스 
void AProductSYSTEM::SpawnNexus(int NexusType, int MonsterIndex)
{
	FName Path = TEXT("Blueprint'/Game/GameSystem/AI/Nexus2/AI_Nexus_BP.AI_Nexus_BP_C'");
	UClass* GeneratedBP = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), NULL, *Path.ToString()));
	FVector location;

	
	//1 왼쪽 2 오른쪽 
	if (NexusType == 1) {
		location = FVector(2031.0, -286.0, 98.0);
		
	}
	else {
		location = FVector(2031.0, -7036.0, 98.0);
	}

	AAI_Nexus *Nexus = GetWorld()->SpawnActor<AAI_Nexus>(GeneratedBP, location, FRotator::ZeroRotator);
	Nexus->monsterIndex = MonsterIndex;
	Nexus->MonsterNowLocation = location;
	Nexus->EnemyType = NexusType;
	Nexus->MonsterMoving = false;
	Nexus->MonsterAlive = false;
	Nexus->tickDeathFlag = false;
	Nexus->MonsterHelath = 400;
	Nexus->MAXMonsterHelath = 400;
	AMonsterBase *mobase = Nexus;
	gameInstance->SpawnMonsterList.Emplace(mobase);
}
