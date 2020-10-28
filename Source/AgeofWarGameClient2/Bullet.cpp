// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "Engine/Classes/Components/SphereComponent.h"
#include "Engine/Classes/GameFramework/ProjectileMovementComponent.h"


// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	CollisionComponent->InitSphereRadius(15.0f);
	RootComponent = CollisionComponent;

	BulletMove = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	BulletMove->SetUpdatedComponent(CollisionComponent);
	BulletMove->InitialSpeed = 4000.0f;
	BulletMove->MaxSpeed = 4000.0f;
	BulletMove->bRotationFollowsVelocity = true;
	BulletMove->bShouldBounce = true;
	BulletMove->Bounciness = 0.3f;

	InitialLifeSpan = 1.0f; // 총알 수명 
}

// 총알이 사라질때실행되면 좋을텐데 
ABullet::~ABullet()
{
	UE_LOG(LogClass, Warning, TEXT("Bullet Destory!!"));

}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABullet::FireInDirection(const FVector& ShootDirection)
{
	BulletMove->Velocity = ShootDirection * BulletMove->InitialSpeed;
}

