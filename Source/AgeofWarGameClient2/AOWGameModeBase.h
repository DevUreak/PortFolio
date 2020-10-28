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
		int Event; // 현재 이벤트 상태 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EVENTMSG")
		FString sessionID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EVENTMSG")
		FString macID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EVENTMSG")
		FString msg; //이벤트 내용?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EVENTMSG")
		int RequestOwnerType = 0; // msg 요청자 구별 식별자 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EVENTMSG")
		int LastMatchingType = 0; // 2차매칭 수락,캔슬 TYPE 1SUCCESS 2CANCLE 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EVENTMSG")
		int CheckFlag; // 프로토콜 마다 다양한 방법으로 해석 됨 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EVENTMSG")
		TArray<FString> ChannelUserList; // 채널별 유저 목록 데이터
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EVENTMSG")
		int ChannelNum; // 채널 번호 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EVENTMSG")
		int PostBoxSize; // post box 사이즈
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
		int EMIndex; // 메세지 고유식별자 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PostBox")
		FString SendDate; // 보낸 날짜
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PostBox")
		FString Sender; // 보낸사람
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PostBox")
		FString Title; // 제목
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PostBox")
		FString Content; // 메세지내용 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PostBox")
		int Type; // 0 노말 1 읽음 2 삭제 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PostBox")
		int AutoDelete; // 자동 삭제 기간
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
		int RankWinRank; // 순위
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ranking")
		float RankWinRate; //  승률 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ranking")
		int RankhighLigth; // 마이포지션 위치 찾기를 실시하였는가 
};


UCLASS()
class AGEOFWARGAMECLIENT2_API AAOWGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override; //제거될떄?
	virtual void BeginPlay() override;
	AAOWGameModeBase(const class FObjectInitializer& ObjectInitializer); // 생성장 변경 http 용?


////////////////////////////////////////////////////////////////////////////////////////////////////////////////// HTTP CODE
public:
	UAOWGameInstance* gameInstance;
	FHttpModule *Http;
	
	UFUNCTION(BlueprintCallable, Category = "http") // 로그인 함수 
		void httpLoginRequest();
	UFUNCTION(BlueprintCallable, Category = "http") //  계정 가입 함수 
		void httpSignUpRequest();
	UFUNCTION(BlueprintCallable, Category = "http") // 우편함 Size 확인
		void httpGetPostBoxSize();
	UFUNCTION(BlueprintCallable, Category = "http") // 우편함 데이터 갱신
		void http_GetPostBoxData();
	UFUNCTION(BlueprintCallable, Category = "http") // 우편함 삭제
		void http_DeletePostBoxData(int EMIndex);
	UFUNCTION(BlueprintCallable, Category = "http") // 우편 read
		void http_ReadPostBoxData(int EMIndex);
	UFUNCTION(BlueprintCallable, Category = "http") // 매칭UI 간편 랭크정보 보기
		void http_RequestSimpleRank();
	UFUNCTION(BlueprintCallable, Category = "http") // 매칭UI 전체 순위 보기 
		void http_RequestTotalRank(int nowTear); 
	UFUNCTION(BlueprintCallable, Category = "http") // 매칭 내 티어 순위 보기 
		void http_RequestTearRank(int nowTear);
	UFUNCTION(BlueprintCallable, Category = "GoogleLogin") // 구글 플레이 로그인 유니크 아이디 GET 
		FString GetGooleoPlayUniqueID(APlayerController* PlayerController);
	UFUNCTION(BlueprintCallable, Category = "GoogleLogin") // 구글 플레이 로그인 닉네임 등록  
		void http_GooleoPlayNameSend(FString name, const FString NetUID);

	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful); // get요청 이후 호출되는 callback 함수 
	void SignUpCheck(TSharedPtr<FJsonObject> JsonObject);
	void LoginCheck(TSharedPtr<FJsonObject> JsonObject);
	void httpHandleGetPostBoxSize(TSharedPtr<FJsonObject> JsonObject, FCustomStruct *Event);
	void GetPostBoxDataHandle(TSharedPtr<FJsonObject> JsonObject, FCustomStruct *Event); //  getpostbox handle
	void DeletePostBoxData(TSharedPtr<FJsonObject> JsonObject, FCustomStruct *Event); // delpost
	void ReadPostBoxData(TSharedPtr<FJsonObject> JsonObject, FCustomStruct *Event);
	void SetSimpleRank(TSharedPtr<FJsonObject> JsonObject, FCustomStruct *Event); // 심플 랭크 처리 
	void SetTotalRank(TSharedPtr<FJsonObject> JsonObject, FCustomStruct *Event); // total 랭크 처리 
	void SetTearRank(TSharedPtr<FJsonObject> JsonObject, FCustomStruct *Event);
	void GooglePlayLoginSuccess(TSharedPtr<FJsonObject> JsonObject); // 구글 플레이 로그인 성공 
	void GooglePlayLoginRegisterSuccess(TSharedPtr<FJsonObject> JsonObject); // 등록된 구글 플레이 로그인 성공 

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PostBoxData")
		TArray<FCustomStructMessage> PostBoxData; // 현재 우편함 데이터
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ranking")
		TArray<FCustomStructRankingMSG> RankingData; // 현재 랭킹 데이터 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "http")
		FString UserMail; // 블루프린트 통신을 위한 변수 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "http")
		FString UserPasswd;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "http")
		FString SignMail;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "http")
		FString SignPasswd;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "http")
		FString SignNickName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "http")
		int SignUpCheckFlag; // 0가입 대기, 1가입 성공 ,2이메일 중복, 3닉네임 중, 4패스워드 정책 실수 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "http")
		int LoginCheckFlag = 0; // 0기본 1성공 2메일불일치 3암호 불일치 


	// 유저 인포 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UserInfo")
		FString SESSION_Eamil; // 세션 유저 닉네임 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UserInfo")
		FString SESSION_NickName; // 세션 유저 닉네임 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UserInfo")
		FString SESSION_UserMacID; // 세션 고유 하드웨어 주소
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UserInfo")
		int SESSION_Tear; // 세션 티어 


/////////////////////////////////////////////////////////////////////////////////////////////////////////UI 핸들링 

public:
	UThreadTaskManager* threadmanager; // 스레드 메니저 각종 독립적인 스레드를 관리할 예정 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventHandler")
		int CountDownValue; // 매칭 타이머 countdown 공유 변수 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EventHandler")
		TArray<FCustomStruct> EventHandlerData; // UI 이벤트 데이터 저장소 
public:
	UFUNCTION(BlueprintCallable, Category = "EventHandler")
		FCustomStruct getEventData(); // UI 이벤트 큐 메소드 
	UFUNCTION(BlueprintCallable, Category = "EventHandler")
		int getEventHandlerSize(); // 현재 이벤트 크기 
	UFUNCTION(BlueprintCallable, Category = "EventHandler")
		void removeEventHandlerData(); // 수행된 첫 이벤트 제거 
	UFUNCTION(BlueprintCallable, Category = "EventHandler")
		void CountDownThreadRun(); // 카운트 다운 스레드 실행 
	UFUNCTION(BlueprintCallable, Category = "EventHandler")
		void RankingHighlightChange(int index); // 랭킹 하이라이트를 바꿔주는 함수 
	

////////////////////////////////////////////////////////////////////////////////////////////////////////// Matching SYSTEM CODE (TCP)

public:
	FSocket* ListenerSocket;
	FSocket* ConnectionSocket;

	FTimerHandle TCPSocketListenerTimerHandle;
	FTimerHandle LastMatchingTimerHandle;

	bool MatChginConnCreateSocket(const FString& YourChosenSocketName, const FString& TheIP, const int32 ThePort);

	FString create_msg_event10();
	FString StringFromBinaryArray(TArray<uint8> BinaryArray); // 데이터 파싱? 

	void TCPSocketListener();		// 스레드로 이벤트 돌리수잇다는데? listen
	bool Parse(const FString& MaskString, FIPv4Address& OutMask); // 없어서 파서 제작
	void recvMatchEventHandle(TSharedPtr<FJsonObject> JsonObject); // recv 메세지 분석
	bool SendMessage(const FString& Message);
	void BinaryArrayFromString(const FString& Message, TArray<uint8>& OutBinaryArray);
	void LastCancleEventProcess(TSharedPtr<FJsonObject> JsonObject, FCustomStruct* event); // 2차 매칭 취소 이벤트 처리 
	void LastSuccessEventProcess(TSharedPtr<FJsonObject> JsonObject, FCustomStruct* event); // 2차 매칭 성공 이벤트 처리 
	void LastMatchingEventHandling(TSharedPtr<FJsonObject> JsonObject, FCustomStruct* event);
	void setGameHandling(TSharedPtr<FJsonObject> JsonObject, FCustomStruct* event); // 게임 시작 핸들링 ㄱㄱ 

	UFUNCTION(BlueprintCallable, Category = "Matching")
		bool ConnMatching();
	UFUNCTION(BlueprintCallable, Category = "Matching")
		void startMatching();
	UFUNCTION(BlueprintCallable, Category = "Matching")
		void CancleMatch();
	UFUNCTION(BlueprintCallable, Category = "Matching")
		void LastCancleMatch(); // 2차 매칭 캔슬 
	UFUNCTION(BlueprintCallable, Category = "Matching")
		void LastSuccessMatch(); // 2차 매칭 수락 
	UFUNCTION(BlueprintCallable, Category = "Matching")
		void LastMatchingResetUI(float start); // 2차매칭 RESTUI

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Matching")
		FString gbOptionString;


	////////////////////////////////////////////////////////////////////////////////////////////////////////// Chatting SYSTEM CODE (TCP)
public:

	FSocket* Chat_ConnectionSocket;
	FTimerHandle Chat_TCPSocketListenerTimerHandle;

	bool Chatting_ConnCreateSocket(const FString& YourChosenSocketName, const FString& TheIP, const int32 ThePort); //연결 소켓 제작 
	void Chatting_TCPSocketListener(); // recv 소켓 
	void ChattingrecvEventHandle(TSharedPtr<FJsonObject> JsonObject);
	bool Chatting_SendMessage(const FString& Message);
	void Chatting_SendUserInfo(TSharedPtr<FJsonObject> JsonObject);
	void setChannelListSize(TSharedPtr<FJsonObject> JsonObject);
	void setChannelUserList(TSharedPtr<FJsonObject> JsonObject, FCustomStruct *event); // 채널별 유저목록 동기화 이벤트 처리
	void firstChannelUserList(TSharedPtr<FJsonObject> JsonObject); // 첫 채널입장 처리 
	void EmergencyMsg(TSharedPtr<FJsonObject> JsonObject, FCustomStruct *event); // 긴급 메세지 처리 
	void setChattingMsg(TSharedPtr<FJsonObject> JsonObject, FCustomStruct *event); // 임시 채팅 메세지 set 


public:
	TArray<int> channellist;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "chatting")
		int nowChannelNum; // 현재 채널 정보 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "chatting")
		TArray<FString> ChannelList; // 채널별 사이즈 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "chatting")
		bool ChannelListCheck; // 채널리스트 얻어왔는지 확인 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "chatting")
		int ChannelListSize; // 채널리스트 얻어왔는지 확인 
	UFUNCTION(BlueprintCallable, Category = "Chatting") // 임시 메시지 핸들러 
		bool ConnChatting(); // 채팅 서버 연결
	UFUNCTION(BlueprintCallable, Category = "chatting")
		void Chatting_sendmsg(FString msg);
	UFUNCTION(BlueprintCallable, Category = "chatting")
		TArray<int>& getChannelList();
	UFUNCTION(BlueprintCallable, Category = "chatting")
		void Chatting_getChannelList(); // 서버로 channel 갯수를 가져온다 
	UFUNCTION(BlueprintCallable, Category = "chatting")
		void removeChannelList(); // 채널 리스트 데이터 제거 
	UFUNCTION(BlueprintCallable, Category = "chatting")
		void moveChannel(int ChannelNum); // 채널 이동후 유저목록 request
	
};
