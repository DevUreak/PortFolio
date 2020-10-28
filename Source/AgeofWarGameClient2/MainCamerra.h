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
	AActor* Owner; // 소유권 얻기 
	AActor* openPlayer; // 첫 플레이어

	int num;//테스트용 변수 
	int posRight;
	int posLeft;
	float wideLeft = -1860.0; // 화면 좌우 넓이 고정비 
	float wideRight = -5180; // 화면 좌우 넓이 고정비 
	FVector location; // 위치 정보 
	FVector location2; // 위치 정보
	FString posCamerra; // 카메라 위치정보 
	FString pos2;


	FVector2D previousTouchLocation; // 터치 이벤트 좌표 변수 
	FVector2D newTouchLocation;
	APlayerController* playerController; // 컨트롤러 
	void OnInputTouchBegin(ETouchIndex::Type touchType, FVector newTouchLocation); //애브리타임 터치스크린에서 확인하기 

protected:
	UPROPERTY(EditAnywhere)
		class USpringArmComponent* OurCameraSpringArm;// 폰에 붙은 카메라 컴포넌트를 유연하게 쫓아다니게 하는 클래스?
	UPROPERTY(EditAnywhere)
		class UCameraComponent* OurCamera;

public:
	UFUNCTION(BlueprintCallable, Category = "Matching")
		void SetSpawnCamerraPosition(int UserType);

};
