// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCamerra.h"
#include "Engine/Classes/GameFramework/SpringArmComponent.h"
#include "Engine/Engine.h"
#include "Engine/Classes/Camera/CameraComponent.h"
#include "Engine/Classes/Components/InputComponent.h"
#include "Engine/Classes/GameFramework/PlayerController.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"

// Sets default values
AMainCamerra::AMainCamerra()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent")); // RootComponent 라는 이름으로 된 "객체"를 생성 
	OurCameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));

	OurCameraSpringArm->SetupAttachment(RootComponent);
	OurCameraSpringArm->SetRelativeLocationAndRotation(FVector(.0f, .0f, 50.f), FRotator(-60.f, .0f, .0f));  // 카메라?위치조정 

	OurCameraSpringArm->TargetArmLength = 200.f;
	OurCameraSpringArm->bEnableCameraLag = true;
	OurCameraSpringArm->CameraLagSpeed = 3.0f;

	OurCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("GameCamera"));
	OurCamera->SetupAttachment(OurCameraSpringArm, USpringArmComponent::SocketName); // ourCamera에 RootComponent를 attach시키는 함수 


	//로컬 플레이어 스폰시 자동으로 폰을 동작 시키도록 설정 
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void AMainCamerra::BeginPlay()
{
	Super::BeginPlay();

	playerController = UGameplayStatics::GetPlayerController(this, 0); // 컨트롤러 초기화?

	Owner = GetOwner(); //owner 얻기 

	openPlayer = GetWorld()->GetFirstPlayerController()->GetPawn();


}

// Called every frame
void AMainCamerra::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	bool bIsCurrentlyPressed;
	playerController->GetInputTouchState(ETouchIndex::Touch1, previousTouchLocation.X, previousTouchLocation.Y, bIsCurrentlyPressed);
	if (bIsCurrentlyPressed) {

		if (previousTouchLocation.Equals(newTouchLocation)) { // 같은 위치 홀드 

			newTouchLocation = previousTouchLocation;
		}
		else {
			location = RootComponent->RelativeLocation;
			if (newTouchLocation.X > previousTouchLocation.X) { // 감소 <- 카메라가 오른쪽으로 가야함
	
				if (location.Y > wideRight)
				{
					location.Y = location.Y - 60;
					RootComponent->SetWorldLocation(location);
				}

			}
			else if (newTouchLocation.X < previousTouchLocation.X) { // 증가 ->  카메라가 왼쪽으로 가야함 
				
				if (location.Y < wideLeft)
				{
					location.Y = location.Y + 60;
					RootComponent->SetWorldLocation(location);
				}

			}
			newTouchLocation = previousTouchLocation;
		}
	}
}

// Called to bind functionality to input
void AMainCamerra::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AMainCamerra::OnInputTouchBegin); // 터치이벤트 바인딩 

	//PlayerInputComponent->BindAxis("A_button",this,&AMainCamerra::moveLeft); // 꼭 바인드할 함수의 인자가(반드시 한개) 있어야함 없으면 에러남!!

}

void AMainCamerra::OnInputTouchBegin(ETouchIndex::Type touchType, FVector newTouchLocation) { // 터치이벤트 실행 함수 

	/*if (touchType == ETouchIndex::Touch1) { // 에디터에 등록한 touch1 인풋값이랑 같다면

		bool bIsCurrentlyPressed;
		playerController->GetInputTouchState(ETouchIndex::Touch1, previousTouchLocation.X, previousTouchLocation.Y, bIsCurrentlyPressed);
	}*/
}


void AMainCamerra::SetSpawnCamerraPosition(int UserType)
{
	FVector camerraLocation;
	if (UserType == 1) {
		camerraLocation.Set(3390.0, -1830.0,1740.0);
		RootComponent->SetWorldLocation(camerraLocation);
	}
	else {
		camerraLocation.Set(3390.0, -5130.0, 1740.0);
		RootComponent->SetWorldLocation(camerraLocation);
	}
}