// Fill out your copyright notice in the Description page of Project Settings.


#include "AI_MurdockChar.h"
#include "Bullet.h"
#include "Engine/Engine.h"
#include "AI_MurdockController.h"



AAI_MurdockChar::AAI_MurdockChar()
{
	//ai ���� ���� �ֽ� 
	bUseControllerRotationYaw = true;
	PrimaryActorTick.bCanEverTick = true;
	//nav ai�� �浹 false 
	//GetCharacterMovement()->NavAgentProps.AgentRadius = 400.f;
	//GetCharacterMovement()->SetUpdateNavAgentWithOwnersCollisions(false);
}

// Called when the game starts or when spawned
void AAI_MurdockChar::BeginPlay()
{
	Super::BeginPlay();
	AIControllerClass = AAI_MurdockController::StaticClass(); // pawn���ٰ� aicon ���� 
	gameInstance = Cast<UAOWGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	lastAttackGameTime = 0;

}

// Called every frame // ���⼭ �ǰ��� Ȯ�� �ϰ� ó���Ѵ�. 
void AAI_MurdockChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if ((0 < this->HitEventList.Num()) && (this->tickDeathFlag == false))  // �ǰ� �̺�Ʈ�� �������� ���� 
	{
		this->MonsterHelath = this->MonsterHelath - this->HitEventList[0].AttackDmg; // �ϴ��� �����ϰ� ���� 

		if (0 >= this->MonsterHelath) // hp�� �����̰ų� �ٴ��϶� �� �׾����� 
		{
			lastAttackGameTime = HitEventList[0].AttackGameTime + 30; // ��Ÿģ���� 
			tickDeathFlag = true; // ���̻� �ǰ� �ý��۽��� x 
			HitEventList.Empty();

			UE_LOG(LogClass, Warning, TEXT("!!!!!!!! -> gametime -> %d lastgametime -> %d "), gameInstance->GameTime, lastAttackGameTime);
			//�״� �ִϸ��̼� ȣ�� or ������ ���� 
		}
		else {
			HitEventList.RemoveAt(0);
			CallBP_RefreshHPBar(this->MonsterHelath); // hp�� 
		}

	}

	// ���⼭gametime�� üũ�Ͽ� destroy ��Ų��. ����ȭ ��ų��� 
	if ((this->tickDeathFlag == true) && (gameInstance->GameTime == (lastAttackGameTime + 20)))
	{
		this->MonsterDeathDestory(); // ���� ȣ�� // ���� �����ɷ� ó���ȴ� 
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
	this->MonsterType = 1; //bear �� 1 
	this->MonsterRange = 1000.0; // ��Ÿ� 150 ������ �����غ����� �̰� �۵��� �ȵǴ°Ű��⵵�� 
	this->MonsterAlive = false; // ����ִ� == false ���� == true
	this->MonsterHelath = 100;
	this->MAXMonsterHelath = 50;
	this->MonsterDamage = 10;
	this->MonsterATTAKING = false;
	this->tickDeathFlag = false;


	const FString command = FString::Printf(TEXT("InitMonsterRef %s"), *FString::FromInt(MonsterIndex));
	FOutputDeviceNull ar;
	this->CallFunctionByNameWithArguments(*command, ar, NULL, true);

}




// �߻� �Լ� 
void AAI_MurdockChar::Fire(FVector Location, FRotator Rotator)
{

	if (BulletClass)
	{
		FVector CameraLocation = Location;
		FRotator CameraRotation = Rotator;
		//GetActorEyesViewPoint(CameraLocation, CameraRotation);

		FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(BulletOffset);
		FRotator MuzzleRotation = Rotator; // CameraRotation // ���⼭ �߻� ��ġ�� ������ ���س����� 

		//MuzzleRotation.Pitch += 10.0f; // �̰� ���Ƿ� ��ġ���� �����ѰŰ��� 
		
		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;
			ABullet* bullet = World->SpawnActor<ABullet>(BulletClass, MuzzleLocation, MuzzleRotation, SpawnParams); // ���������� getworld�� �ٲ㺸�� 
			if (bullet)
			{
				FVector LaunchDirection = MuzzleRotation.Vector();
				bullet->FireInDirection(LaunchDirection); //�߻� �ִϸ��̼� 
				UE_LOG(LogClass, Warning, TEXT("FIRE!!!!!!!!!!!!!!!!!!!!!!!!!!! "));
			}
		}
	}



}
