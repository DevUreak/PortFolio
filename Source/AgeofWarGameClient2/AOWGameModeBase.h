// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Dom/JsonObject.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "Runtime/Networking/Public/Networking.h"
#include "Blueprint/UserWidget.h"
#include "ThreadTaskManager.h"
#include "AOWGameInstance.h"
#include "vector"
#include "time.h"
#include "stdlib.h"
#include "map"
#include "AOWGameModeBase.generated.h"

using namespace::std;

/**
 * 
 */

USTRUCT(Atomic, BlueprintType)
struct FCustomStruct
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EVENTMSG")
		int Event; // ���� �̺�Ʈ ���� 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EVENTMSG")
		FString sessionID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EVENTMSG")
		FString macID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EVENTMSG")
		FString msg; //�̺�Ʈ ����?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EVENTMSG")
		int RequestOwnerType = 0; // msg ��û�� ���� �ĺ��� 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EVENTMSG")
		int LastMatchingType = 0; // 2����Ī ����,ĵ�� TYPE 1SUCCESS 2CANCLE 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EVENTMSG")
		int CheckFlag; // �������� ���� �پ��� ������� �ؼ� �� 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EVENTMSG")
		TArray<FString> ChannelUserList; // ä�κ� ���� ��� ������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EVENTMSG")
		int ChannelNum; // ä�� ��ȣ 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EVENTMSG")
		int PostBoxSize; // post box ������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EVENTMSG")
		FString RankNickName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EVENTMSG")
		int RankTear;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EVENTMSG")
		int RankWin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EVENTMSG")
		int RankTotal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EVENTMSG")
		int RankScore;
};

USTRUCT(Atomic, BlueprintType)
struct FCustomStructMessage
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PostBox")
		int EMIndex; // �޼��� �����ĺ��� 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PostBox")
		FString SendDate; // ���� ��¥
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PostBox")
		FString Sender; // �������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PostBox")
		FString Title; // ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PostBox")
		FString Content; // �޼������� 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PostBox")
		int Type; // 0 �븻 1 ���� 2 ���� 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PostBox")
		int AutoDelete; // �ڵ� ���� �Ⱓ
};

USTRUCT(Atomic, BlueprintType)
struct FCustomStructRankingMSG
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ranking")
		FString RankNickName; //
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ranking")
		int RankTear; //
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ranking")
		int RankScore; // 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ranking")
		int RankWinRank; // ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ranking")
		float RankWinRate; //  �·� 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ranking")
		int RankhighLigth; // ���������� ��ġ ã�⸦ �ǽ��Ͽ��°� 
};


UCLASS()
class AGEOFWARGAMECLIENT2_API AAOWGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override; //���ŵɋ�?
	virtual void BeginPlay() override;
	AAOWGameModeBase(const class FObjectInitializer& ObjectInitializer); // ������ ���� http ��?


////////////////////////////////////////////////////////////////////////////////////////////////////////////////// HTTP CODE
public:
	UAOWGameInstance* gameInstance;
	FHttpModule *Http;
	
	UFUNCTION(BlueprintCallable, Category = "http") // �α��� �Լ� 
		void httpLoginRequest();
	UFUNCTION(BlueprintCallable, Category = "http") //  ���� ���� �Լ� 
		void httpSignUpRequest();
	UFUNCTION(BlueprintCallable, Category = "http") // ������ Size Ȯ��
		void httpGetPostBoxSize();
	UFUNCTION(BlueprintCallable, Category = "http") // ������ ������ ����
		void http_GetPostBoxData();
	UFUNCTION(BlueprintCallable, Category = "http") // ������ ����
		void http_DeletePostBoxData(int EMIndex);
	UFUNCTION(BlueprintCallable, Category = "http") // ���� read
		void http_ReadPostBoxData(int EMIndex);
	UFUNCTION(BlueprintCallable, Category = "http") // ��ĪUI ���� ��ũ���� ����
		void http_RequestSimpleRank();
	UFUNCTION(BlueprintCallable, Category = "http") // ��ĪUI ��ü ���� ���� 
		void http_RequestTotalRank(int nowTear); 
	UFUNCTION(BlueprintCallable, Category = "http") // ��Ī �� Ƽ�� ���� ���� 
		void http_RequestTearRank(int nowTear);
	UFUNCTION(BlueprintCallable, Category = "GoogleLogin") // ���� �÷��� �α��� ����ũ ���̵� GET 
		FString GetGooleoPlayUniqueID(APlayerController* PlayerController);
	UFUNCTION(BlueprintCallable, Category = "GoogleLogin") // ���� �÷��� �α��� �г��� ���  
		void http_GooleoPlayNameSend(FString name, const FString NetUID);

	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful); // get��û ���� ȣ��Ǵ� callback �Լ� 
	void SignUpCheck(TSharedPtr<FJsonObject> JsonObject);
	void LoginCheck(TSharedPtr<FJsonObject> JsonObject);
	void httpHandleGetPostBoxSize(TSharedPtr<FJsonObject> JsonObject, FCustomStruct *Event);
	void GetPostBoxDataHandle(TSharedPtr<FJsonObject> JsonObject, FCustomStruct *Event); //  getpostbox handle
	void DeletePostBoxData(TSharedPtr<FJsonObject> JsonObject, FCustomStruct *Event); // delpost
	void ReadPostBoxData(TSharedPtr<FJsonObject> JsonObject, FCustomStruct *Event);
	void SetSimpleRank(TSharedPtr<FJsonObject> JsonObject, FCustomStruct *Event); // ���� ��ũ ó�� 
	void SetTotalRank(TSharedPtr<FJsonObject> JsonObject, FCustomStruct *Event); // total ��ũ ó�� 
	void SetTearRank(TSharedPtr<FJsonObject> JsonObject, FCustomStruct *Event);
	void GooglePlayLoginSuccess(TSharedPtr<FJsonObject> JsonObject); // ���� �÷��� �α��� ���� 
	void GooglePlayLoginRegisterSuccess(TSharedPtr<FJsonObject> JsonObject); // ��ϵ� ���� �÷��� �α��� ���� 

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PostBoxData")
		TArray<FCustomStructMessage> PostBoxData; // ���� ������ ������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ranking")
		TArray<FCustomStructRankingMSG> RankingData; // ���� ��ŷ ������ 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "http")
		FString UserMail; // �������Ʈ ����� ���� ���� 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "http")
		FString UserPasswd;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "http")
		FString SignMail;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "http")
		FString SignPasswd;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "http")
		FString SignNickName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "http")
		int SignUpCheckFlag; // 0���� ���, 1���� ���� ,2�̸��� �ߺ�, 3�г��� ��, 4�н����� ��å �Ǽ� 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "http")
		int LoginCheckFlag = 0; // 0�⺻ 1���� 2���Ϻ���ġ 3��ȣ ����ġ 


	// ���� ���� 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UserInfo")
		FString SESSION_Eamil; // ���� ���� �г��� 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UserInfo")
		FString SESSION_NickName; // ���� ���� �г��� 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UserInfo")
		FString SESSION_UserMacID; // ���� ���� �ϵ���� �ּ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UserInfo")
		int SESSION_Tear; // ���� Ƽ�� 


/////////////////////////////////////////////////////////////////////////////////////////////////////////UI �ڵ鸵 

public:
	UThreadTaskManager* threadmanager; // ������ �޴��� ���� �������� �����带 ������ ���� 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventHandler")
		int CountDownValue; // ��Ī Ÿ�̸� countdown ���� ���� 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventHandler")
		TArray<FCustomStruct> EventHandlerData; // UI �̺�Ʈ ������ ����� 
public:
	UFUNCTION(BlueprintCallable, Category = "EventHandler")
		FCustomStruct getEventData(); // UI �̺�Ʈ ť �޼ҵ� 
	UFUNCTION(BlueprintCallable, Category = "EventHandler")
		int getEventHandlerSize(); // ���� �̺�Ʈ ũ�� 
	UFUNCTION(BlueprintCallable, Category = "EventHandler")
		void removeEventHandlerData(); // ����� ù �̺�Ʈ ���� 
	UFUNCTION(BlueprintCallable, Category = "EventHandler")
		void CountDownThreadRun(); // ī��Ʈ �ٿ� ������ ���� 
	UFUNCTION(BlueprintCallable, Category = "EventHandler")
		void RankingHighlightChange(int index); // ��ŷ ���̶���Ʈ�� �ٲ��ִ� �Լ� 
	

////////////////////////////////////////////////////////////////////////////////////////////////////////// Matching SYSTEM CODE (TCP)

public:
	FSocket* ListenerSocket;
	FSocket* ConnectionSocket;

	FTimerHandle TCPSocketListenerTimerHandle;
	FTimerHandle LastMatchingTimerHandle;

	bool MatChginConnCreateSocket(const FString& YourChosenSocketName, const FString& TheIP, const int32 ThePort);

	FString create_msg_event10();
	FString StringFromBinaryArray(TArray<uint8> BinaryArray); // ������ �Ľ�? 

	void TCPSocketListener();		// ������� �̺�Ʈ �������մٴµ�? listen
	bool Parse(const FString& MaskString, FIPv4Address& OutMask); // ��� �ļ� ����
	void recvMatchEventHandle(TSharedPtr<FJsonObject> JsonObject); // recv �޼��� �м�
	bool SendMessage(const FString& Message);
	void BinaryArrayFromString(const FString& Message, TArray<uint8>& OutBinaryArray);
	void LastCancleEventProcess(TSharedPtr<FJsonObject> JsonObject, FCustomStruct* event); // 2�� ��Ī ��� �̺�Ʈ ó�� 
	void LastSuccessEventProcess(TSharedPtr<FJsonObject> JsonObject, FCustomStruct* event); // 2�� ��Ī ���� �̺�Ʈ ó�� 
	void LastMatchingEventHandling(TSharedPtr<FJsonObject> JsonObject, FCustomStruct* event);
	void setGameHandling(TSharedPtr<FJsonObject> JsonObject, FCustomStruct* event); // ���� ���� �ڵ鸵 ���� 

	UFUNCTION(BlueprintCallable, Category = "Matching")
		bool ConnMatching();
	UFUNCTION(BlueprintCallable, Category = "Matching")
		void startMatching();
	UFUNCTION(BlueprintCallable, Category = "Matching")
		void CancleMatch();
	UFUNCTION(BlueprintCallable, Category = "Matching")
		void LastCancleMatch(); // 2�� ��Ī ĵ�� 
	UFUNCTION(BlueprintCallable, Category = "Matching")
		void LastSuccessMatch(); // 2�� ��Ī ���� 
	UFUNCTION(BlueprintCallable, Category = "Matching")
		void LastMatchingResetUI(float start); // 2����Ī RESTUI

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Matching")
		FString gbOptionString;


	////////////////////////////////////////////////////////////////////////////////////////////////////////// Chatting SYSTEM CODE (TCP)
public:

	FSocket* Chat_ConnectionSocket;
	FTimerHandle Chat_TCPSocketListenerTimerHandle;

	bool Chatting_ConnCreateSocket(const FString& YourChosenSocketName, const FString& TheIP, const int32 ThePort); //���� ���� ���� 
	void Chatting_TCPSocketListener(); // recv ���� 
	void ChattingrecvEventHandle(TSharedPtr<FJsonObject> JsonObject);
	bool Chatting_SendMessage(const FString& Message);
	void Chatting_SendUserInfo(TSharedPtr<FJsonObject> JsonObject);
	void setChannelListSize(TSharedPtr<FJsonObject> JsonObject);
	void setChannelUserList(TSharedPtr<FJsonObject> JsonObject, FCustomStruct *event); // ä�κ� ������� ����ȭ �̺�Ʈ ó��
	void firstChannelUserList(TSharedPtr<FJsonObject> JsonObject); // ù ä������ ó�� 
	void EmergencyMsg(TSharedPtr<FJsonObject> JsonObject, FCustomStruct *event); // ��� �޼��� ó�� 
	void setChattingMsg(TSharedPtr<FJsonObject> JsonObject, FCustomStruct *event); // �ӽ� ä�� �޼��� set 


public:
	TArray<int> channellist;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "chatting")
		int nowChannelNum; // ���� ä�� ���� 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "chatting")
		TArray<FString> ChannelList; // ä�κ� ������ 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "chatting")
		bool ChannelListCheck; // ä�θ���Ʈ ���Դ��� Ȯ�� 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "chatting")
		int ChannelListSize; // ä�θ���Ʈ ���Դ��� Ȯ�� 
	UFUNCTION(BlueprintCallable, Category = "Chatting") // �ӽ� �޽��� �ڵ鷯 
		bool ConnChatting(); // ä�� ���� ����
	UFUNCTION(BlueprintCallable, Category = "chatting")
		void Chatting_sendmsg(FString msg);
	UFUNCTION(BlueprintCallable, Category = "chatting")
		TArray<int>& getChannelList();
	UFUNCTION(BlueprintCallable, Category = "chatting")
		void Chatting_getChannelList(); // ������ channel ������ �����´� 
	UFUNCTION(BlueprintCallable, Category = "chatting")
		void removeChannelList(); // ä�� ����Ʈ ������ ���� 
	UFUNCTION(BlueprintCallable, Category = "chatting")
		void moveChannel(int ChannelNum); // ä�� �̵��� ������� request
	
};
