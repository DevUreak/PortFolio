// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MainCamerra.generated.h"

UCLASS()
class AGEOFWARGAMECLIENT2_API AMainCamerra : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMainCamerra();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	//void whatNameis();


public:
	AActor* Owner; // ������ ��� 
	AActor* openPlayer; // ù �÷��̾�

	int num;//�׽�Ʈ�� ���� 
	int posRight;
	int posLeft;
	float wideLeft = -1860.0; // ȭ�� �¿� ���� ������ 
	float wideRight = -5180; // ȭ�� �¿� ���� ������ 
	FVector location; // ��ġ ���� 
	FVector location2; // ��ġ ����
	FString posCamerra; // ī�޶� ��ġ���� 
	FString pos2;


	FVector2D previousTouchLocation; // ��ġ �̺�Ʈ ��ǥ ���� 
	FVector2D newTouchLocation;
	APlayerController* playerController; // ��Ʈ�ѷ� 
	void OnInputTouchBegin(ETouchIndex::Type touchType, FVector newTouchLocation); //�ֺ긮Ÿ�� ��ġ��ũ������ Ȯ���ϱ� 

protected:
	UPROPERTY(EditAnywhere)
		class USpringArmComponent* OurCameraSpringArm;// ���� ���� ī�޶� ������Ʈ�� �����ϰ� �Ѿƴٴϰ� �ϴ� Ŭ����?
	UPROPERTY(EditAnywhere)
		class UCameraComponent* OurCamera;

public:
	UFUNCTION(BlueprintCallable, Category = "Matching")
		void SetSpawnCamerraPosition(int UserType);

};
