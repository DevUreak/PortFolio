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

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent")); // RootComponent ��� �̸����� �� "��ü"�� ���� 
	OurCameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));

	OurCameraSpringArm->SetupAttachment(RootComponent);
	OurCameraSpringArm->SetRelativeLocationAndRotation(FVector(.0f, .0f, 50.f), FRotator(-60.f, .0f, .0f));  // ī�޶�?��ġ���� 

	OurCameraSpringArm->TargetArmLength = 200.f;
	OurCameraSpringArm->bEnableCameraLag = true;
	OurCameraSpringArm->CameraLagSpeed = 3.0f;

	OurCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("GameCamera"));
	OurCamera->SetupAttachment(OurCameraSpringArm, USpringArmComponent::SocketName); // ourCamera�� RootComponent�� attach��Ű�� �Լ� 


	//���� �÷��̾� ������ �ڵ����� ���� ���� ��Ű���� ���� 
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void AMainCamerra::BeginPlay()
{
	Super::BeginPlay();

	playerController = UGameplayStatics::GetPlayerController(this, 0); // ��Ʈ�ѷ� �ʱ�ȭ?

	Owner = GetOwner(); //owner ��� 

	openPlayer = GetWorld()->GetFirstPlayerController()->GetPawn();


}

// Called every frame
void AMainCamerra::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	bool bIsCurrentlyPressed;
	playerController->GetInputTouchState(ETouchIndex::Touch1, previousTouchLocation.X, previousTouchLocation.Y, bIsCurrentlyPressed);
	if (bIsCurrentlyPressed) {

		if (previousTouchLocation.Equals(newTouchLocation)) { // ���� ��ġ Ȧ�� 

			newTouchLocation = previousTouchLocation;
		}
		else {
			location = RootComponent->RelativeLocation;
			if (newTouchLocation.X > previousTouchLocation.X) { // ���� <- ī�޶� ���������� ������
	
				if (location.Y > wideRight)
				{
					location.Y = location.Y - 60;
					RootComponent->SetWorldLocation(location);
				}

			}
			else if (newTouchLocation.X < previousTouchLocation.X) { // ���� ->  ī�޶� �������� ������ 
				
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
	PlayerInputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AMainCamerra::OnInputTouchBegin); // ��ġ�̺�Ʈ ���ε� 

	//PlayerInputComponent->BindAxis("A_button",this,&AMainCamerra::moveLeft); // �� ���ε��� �Լ��� ���ڰ�(�ݵ�� �Ѱ�) �־���� ������ ������!!

}

void AMainCamerra::OnInputTouchBegin(ETouchIndex::Type touchType, FVector newTouchLocation) { // ��ġ�̺�Ʈ ���� �Լ� 

	/*if (touchType == ETouchIndex::Touch1) { // �����Ϳ� ����� touch1 ��ǲ���̶� ���ٸ�

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