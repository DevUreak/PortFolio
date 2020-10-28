// Fill out your copyright notice in the Description page of Project Settings.


#include "AI_Nexus.h"
#include "Components/SphereComponent.h"
#include "AI_NexusController.h"

// Sets default values
AAI_Nexus::AAI_Nexus()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = true;
	this->MonsterType = 0; // 0 �ؼ��� 

}

// Called when the game starts or when spawned
void AAI_Nexus::BeginPlay()
{
	Super::BeginPlay();
	gameInstance = Cast<UAOWGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
}

// Called every frame
void AAI_Nexus::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if ((0 < this->HitEventList.Num()) && (this->tickDeathFlag == false))  // �ǰ� �̺�Ʈ�� �������� ���� 
	{
		this->MonsterHelath = this->MonsterHelath -20; // �ϴ��� �����ϰ� ���� 

		UE_LOG(LogClass, Warning, TEXT(" attack! dmg -> %d "), this->HitEventList[0].AttackDmg);
	
		if (0 >= this->MonsterHelath) // hp�� �����̰ų� �ٴ��϶� �� �׾����� 
		{
			lastAttackGameTime = HitEventList[0].AttackGameTime + 30; // ��Ÿģ���� 
			tickDeathFlag = true; // ���̻� �ǰ� �ý��۽��� x 
			gameInstance->GameResultUser = this->EnemyType;
			gameInstance->GameResultFlag = true; // ���� ������ �˸� 
			HitEventList.Empty();
		}
		else {
			HitEventList.RemoveAt(0);
			CallBP_RefreshHPBar(this->MonsterHelath);
		}

	}

}
 
// Called to bind functionality to input
void AAI_Nexus::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	AIControllerClass = AAI_NexusController::StaticClass(); // pawn���ٰ� aicon ���� 
}

void AAI_Nexus::CallBP_RefreshHPBar(int nowHelath)
{
	const FString command2 = FString::Printf(TEXT("RefreshHPBar2 %s"), *FString::FromInt(nowHelath));
	FOutputDeviceNull ar;
	this->CallFunctionByNameWithArguments(*command2, ar, NULL, true);
}
