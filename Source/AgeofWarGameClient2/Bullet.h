// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

UCLASS()
class AGEOFWARGAMECLIENT2_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABullet();
	~ABullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void FireInDirection(const FVector& ShootDirection); // 발사체 발사하는 함수 



public:
	UPROPERTY(VisibleDefaultsOnly, Category = "Bullet")
		class USphereComponent* CollisionComponent;
	UPROPERTY(VisibleAnywhere, Category = "Movement")
		class UProjectileMovementComponent* BulletMove;

	



};
