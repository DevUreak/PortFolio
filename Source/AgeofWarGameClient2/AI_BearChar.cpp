// Fill out your copyright notice in the Description page of Project Settings.


#include "AI_BearChar.h"
#include "AI_BearController.h"


AAI_BearChar::AAI_BearChar() 
{
	//ai 방향 전방 주시 
	bUseControllerRotationYaw = true;
	PrimaryActorTick.bCanEverTick = true;
	//nav ai들 충돌 false 
	//GetCharacterMovement()->NavAgentProps.AgentRadius = 400.f;
	//GetCharacterMovement()->SetUpdateNavAgentWithOwnersCollisions(false);
}

// Called when the game starts or when spawned
void AAI_BearChar::BeginPlay()
{
	Super::BeginPlay();
	AIControllerClass = AAI_BearController::StaticClass(); // pawn에다가 aicon 셋팅 
	gameInstance = Cast<UAOWGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	lastAttackGameTime = 0;

}

// Called every frame // 여기서 피격을 확인 하고 처리한다. 
void AAI_BearChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if ((0<this->HitEventList.Num()) && (this->tickDeathFlag==false))  // 피격 이벤트가 들어왔을때 실행 
	{
		this->MonsterHelath = this->MonsterHelath - this->HitEventList[0].AttackDmg; // 일단은 공격하고 보자 
	
		if(0 >= this->MonsterHelath) // hp가 음수이거나 바닥일때 즉 죽었을때 
		{
			lastAttackGameTime = HitEventList[0].AttackGameTime+30; // 막타친새끼 
			tickDeathFlag = true; // 더이상 피격 시스템실행 x 
			HitEventList.Empty();

			UE_LOG(LogClass, Warning, TEXT("!!!!!!!! -> gametime -> %d lastgametime -> %d "), gameInstance->GameTime, lastAttackGameTime);
			//죽는 애니메이션 호출 or 변수값 수정 
		}
		else {
			HitEventList.RemoveAt(0);
			CallBP_RefreshHPBar(this->MonsterHelath); // hp바 
		}
		
	}

	// 여기서gametime을 체크하여 destroy 시킨다. 동기화 시킬라고 
	if((this->tickDeathFlag==true) && (gameInstance->GameTime == (lastAttackGameTime+20)))
	{
		this->MonsterDeathDestory(); // 데스 호출 // 드디어 죽은걸로 처리된다 
	}


}


void AAI_BearChar::CallBP_ChagneHPBar(int EnemyType) 
{

	const FString command = FString::Printf(TEXT("ChangeHPBar %s %s"), *FString::FromInt(EnemyType), *FString::FromInt(this->MAXMonsterHelath));
	FOutputDeviceNull ar;
	this->CallFunctionByNameWithArguments(*command, ar, NULL,true);
	
}

void AAI_BearChar::CallBP_RefreshHPBar(int nowHelath)
{
	const FString command2 = FString::Printf(TEXT("RefreshHPBar %s"),*FString::FromInt(nowHelath));
	FOutputDeviceNull ar;
	this->CallFunctionByNameWithArguments(*command2, ar, NULL, true);
}

void AAI_BearChar::InitBearInfo(int MonsterIndex) 
{
	this->MonsterType = 1; //bear 는 1 
	this->MonsterRange = 350.0; // 사거리 150
	this->MonsterAlive = false; // 살아있다 == false 죽음 == true
	this->MonsterHelath = 150;
	this->MAXMonsterHelath = 150;
	this->MonsterDamage = 20;
	this->MonsterATTAKING = false;
	this->tickDeathFlag = false;
		

	const FString command = FString::Printf(TEXT("InitMonsterRef %s"), *FString::FromInt(MonsterIndex));
	FOutputDeviceNull ar;
	this->CallFunctionByNameWithArguments(*command, ar, NULL, true);

}