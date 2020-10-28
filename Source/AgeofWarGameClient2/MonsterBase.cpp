// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterBase.h"

// Sets default values
AMonsterBase::AMonsterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AMonsterBase::BeginPlay()
{
	Super::BeginPlay();
	Anim_MonsterAction = 0;
}

// Called every frame
void AMonsterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMonsterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// Ÿ�� �������� �޾Ƽ� �Ÿ� ��� 
float AMonsterBase::SearchforTarget(FVector myLocation, FVector targetLocation)
{
	float distance;
	FVector2D myLocation2D = FVector2D(myLocation.X, myLocation.Y);
	FVector2D targetLocation2D = FVector2D(targetLocation.X, targetLocation.Y);
	distance = FVector2D::Distance(myLocation2D, targetLocation2D);
	return distance;
	
}


//�׽�Ʈ�� ��� �Լ� ȣ�� 
void AMonsterBase::MonsterDeathDestory() {
	this->MonsterAlive = true;	 // Search task���� ���̻� ��ġ�������ϰ� ����  or ���� �ִϸ��̼� ���� 
	this->tickDeathFlag = false; // ���̻� ƽ ���� x 
	UE_LOG(LogClass, Warning, TEXT(" destory!"));
}

void AMonsterBase::MonsterDeathDestoryFinal() 
{
	this->K2_DestroyActor();
	UE_LOG(LogClass, Warning, TEXT("Last Destory!!"));
}