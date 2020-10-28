// Fill out your copyright notice in the Description page of Project Settings.


#include "AI_MurdockChar.h"
#include "Bullet.h"
#include "Engine/Engine.h"
#include "AI_MurdockController.h"



AAI_MurdockChar::AAI_MurdockChar()
{
	//ai 방향 전방 주시 
	bUseControllerRotationYaw = true;
	PrimaryActorTick.bCanEverTick = true;
	//nav ai들 충돌 false 
	//GetCharacterMovement()->NavAgentProps.AgentRadius = 400.f;
	//GetCharacterMovement()->SetUpdateNavAgentWithOwnersCollisions(false);
}

// Called when the game starts or when spawned
void AAI_MurdockChar::BeginPlay()
{
	Super::BeginPlay();
	AIControllerClass = AAI_MurdockController::StaticClass(); // pawn에다가 aicon 셋팅 
	gameInstance = Cast<UAOWGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	lastAttackGameTime = 0;

}

// Called every frame // 여기서 피격을 확인 하고 처리한다. 
void AAI_MurdockChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if ((0 < this->HitEventList.Num()) && (this->tickDeathFlag == false))  // 피격 이벤트가 들어왔을때 실행 
	{
		this->MonsterHelath = this->MonsterHelath - this->HitEventList[0].AttackDmg; // 일단은 공격하고 보자 

		if (0 >= this->MonsterHelath) // hp가 음수이거나 바닥일때 즉 죽었을때 
		{
			lastAttackGameTime = HitEventList[0].AttackGameTime + 30; // 막타친새끼 
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
	if ((this->tickDeathFlag == true) && (gameInstance->GameTime == (lastAttackGameTime + 20)))
	{
		this->MonsterDeathDestory(); // 데스 호출 // 드디어 죽은걸로 처리된다 
	}


}


void AAI_MurdockChar::CallBP_ChagneHPBar(int EnemyType)
{
	const FString command = FString::Printf(TEXT("ChangeHPBar %s %s"), *FString::FromInt(EnemyType), *FString::FromInt(this->MAXMonsterHelath));
	FOutputDeviceNull ar;
	this->CallFunctionByNameWithArguments(*command, ar, NULL, true);

}

void AAI_MurdockChar::CallBP_RefreshHPBar(int nowHelath)
{
	const FString command2 = FString::Printf(TEXT("RefreshHPBar %s"), *FString::FromInt(nowHelath));
	FOutputDeviceNull ar;
	this->CallFunctionByNameWithArguments(*command2, ar, NULL, true);
}

void AAI_MurdockChar::InitMurdockInfo(int MonsterIndex)
{
	this->MonsterType = 1; //bear 는 1 
	this->MonsterRange = 1000.0; // 사거리 150 고려를 많이해봐야햄 이거 작동이 안되는거같기도함 
	this->MonsterAlive = false; // 살아있다 == false 죽음 == true
	this->MonsterHelath = 100;
	this->MAXMonsterHelath = 50;
	this->MonsterDamage = 10;
	this->MonsterATTAKING = false;
	this->tickDeathFlag = false;


	const FString command = FString::Printf(TEXT("InitMonsterRef %s"), *FString::FromInt(MonsterIndex));
	FOutputDeviceNull ar;
	this->CallFunctionByNameWithArguments(*command, ar, NULL, true);

}




// 발사 함수 
void AAI_MurdockChar::Fire(FVector Location, FRotator Rotator)
{

	if (BulletClass)
	{
		FVector CameraLocation = Location;
		FRotator CameraRotation = Rotator;
		//GetActorEyesViewPoint(CameraLocation, CameraRotation);

		FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(BulletOffset);
		FRotator MuzzleRotation = Rotator; // CameraRotation // 여기서 발사 위치와 방향을 구해내야함 

		//MuzzleRotation.Pitch += 10.0f; // 이건 임의로 위치값을 조정한거같음 
		
		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;
			ABullet* bullet = World->SpawnActor<ABullet>(BulletClass, MuzzleLocation, MuzzleRotation, SpawnParams); // 문제있으면 getworld로 바꿔보자 
			if (bullet)
			{
				FVector LaunchDirection = MuzzleRotation.Vector();
				bullet->FireInDirection(LaunchDirection); //발사 애니메이션 
				UE_LOG(LogClass, Warning, TEXT("FIRE!!!!!!!!!!!!!!!!!!!!!!!!!!! "));
			}
		}
	}



}

