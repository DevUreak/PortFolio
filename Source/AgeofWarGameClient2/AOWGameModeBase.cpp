// Fill out your copyright notice in the Description page of Project Settings.


#include "AOWGameModeBase.h"
#include "Templates/SharedPointer.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Engine/World.h"
#include "Templates/SharedPointer.h"
#include "Networking/Public/Networking.h"
#include "Networking/Public/Interfaces/IPv4/IPv4Address.h"
#include "Networking/Public/Interfaces/IPv4/IPv4SubnetMask.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"
#include "TimerManager.h"
#include "ThreadTaskManager.h"
#include "CountDownThread.h"
#include "time.h"
#include "string"
#include "stdlib.h"
#include "Engine/Engine.h"
#include "Misc/App.h"
#include "vector"
#include "map"
#include "Blueprint/UserWidget.h"
#include "AOWGameInstance.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "FSocketMessageHeader.h"


static FGuid SessionID = FGuid::NewGuid(); //세션 아이디 등록
static char RecvDelimiter = '}';
uint32 FSocketMessageHeader::DefaultMagic = 0x9E2B83C1;

AAOWGameModeBase::AAOWGameModeBase(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Http = &FHttpModule::Get();
	
}

void AAOWGameModeBase::BeginPlay()
{
	UserMail = NULL;
	UserPasswd = NULL;
	SignMail = NULL;
	SignPasswd = NULL;
	SignNickName = NULL;
	SESSION_NickName = NULL;
	SESSION_UserMacID = NULL;
	SESSION_Eamil = NULL;
	ChannelListCheck = true;
	SignUpCheckFlag = 0;
	LoginCheckFlag = 0;
	nowChannelNum = -1; // 채널 정보 
	gameInstance = Cast<UAOWGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

}

//IP Parser
bool AAOWGameModeBase::Parse(const FString& MaskString, FIPv4Address& OutMask)
{
	TArray<FString> Tokens;

	if (MaskString.ParseIntoArray(Tokens, TEXT("."), false) == 4)
	{
		OutMask.A = FCString::Atoi(*Tokens[0]);
		OutMask.B = FCString::Atoi(*Tokens[1]);
		OutMask.C = FCString::Atoi(*Tokens[2]);
		OutMask.D = FCString::Atoi(*Tokens[3]);

		return true;
	}

	return false;
}

void AAOWGameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason) {

	Super::EndPlay(EndPlayReason);
	UWorld* World = GetWorld();
	GetWorld()->GetTimerManager().ClearTimer(TCPSocketListenerTimerHandle); //recv 핸들러 제거 
	GetWorld()->GetTimerManager().ClearTimer(Chat_TCPSocketListenerTimerHandle);

	if (ConnectionSocket != NULL) { //모든 소켓 제거
		ConnectionSocket->Close();
		UE_LOG(LogClass, Warning, TEXT("Call Close CONNSOCKET!!!!"));
	}

}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////HTTP CODE


//회원 가입 확인 메소드 
void AAOWGameModeBase::SignUpCheck(TSharedPtr<FJsonObject> JsonObject) {

	FString tmp = JsonObject->GetStringField("SignUpCheck");
	SignUpCheckFlag = FCString::Atoi(*tmp);
}
//로그인 확인 메소드 
void AAOWGameModeBase::LoginCheck(TSharedPtr<FJsonObject> JsonObject) {

	FString tmpFlag = JsonObject->GetStringField("LoginCheck");
	FString tmpEmail;
	FString tmpNickName;
	int tmpTear;
	LoginCheckFlag = FCString::Atoi(*tmpFlag);

	if (LoginCheckFlag == 2) // 로그인성공 // 세션 정보 저장 
	{
		tmpEmail = JsonObject->GetStringField("LoginEmail");
		tmpNickName = JsonObject->GetStringField("LoginNickName");
		tmpTear = JsonObject->GetIntegerField("LoginTear");
		SESSION_Eamil = tmpEmail;
		SESSION_NickName = tmpNickName;
		SESSION_Tear = tmpTear;
	}else { 
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Password DisMatch !!!!")));
	}

	UE_LOG(LogClass, Warning, TEXT("Login Check?"));
	//UE_LOG(LogClass, Error, TEXT("Result -------------111---> %d"), SESSION_Tear);
}

//우편함 size setting 
void AAOWGameModeBase::httpHandleGetPostBoxSize(TSharedPtr<FJsonObject> JsonObject, FCustomStruct *event)
{
	int PostSize = JsonObject->GetIntegerField("PostSize");
	event->Event = 17;
	event->PostBoxSize = PostSize;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("size-> %d"), PostSize));
}

//우편한 set 데이터 핸들링
void AAOWGameModeBase::GetPostBoxDataHandle(TSharedPtr<FJsonObject> JsonObject, FCustomStruct *event)
{
	event->Event = 18;
	PostBoxData.Empty();
	TArray<TSharedPtr<FJsonValue>> Delarr = JsonObject->GetArrayField("DeleteArray");
	TArray<TSharedPtr<FJsonValue>> Readarr = JsonObject->GetArrayField("ReadArray");
	TArray<TSharedPtr<FJsonValue>> Datarr = JsonObject->GetArrayField("PostData");


	for (int i = 0; i < Datarr.Num(); i++) //배열에 인수가 몇개인지 알수있고 
	{
		FCustomStructMessage msg;
		TArray<TSharedPtr<FJsonValue>> postData = Datarr[i]->AsArray();
		msg.EMIndex = postData[0]->AsNumber();
		msg.SendDate = postData[1]->AsString();
		msg.Sender = postData[2]->AsString();
		msg.Title = postData[3]->AsString();
		msg.Content = postData[4]->AsString();
		msg.Type = postData[5]->AsNumber();
		msg.AutoDelete = postData[6]->AsNumber();
		PostBoxData.Emplace(msg);
	}

	if (Readarr.Num() != 0)
	{
		int readSize = Readarr.Num();
		int j = 0;

		while (j < PostBoxData.Num())
		{
			int index = Readarr[(readSize - 1)]->AsNumber();

			if (PostBoxData[j].EMIndex == index) // index가 같다면 
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("del log -> %d, read Size -> %d"), j, readSize));
				PostBoxData[j].Type = 1;
				readSize = readSize - 1;
				j = 0;
				if (readSize == 0)
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("break")));
					break;
				}
				else
					continue;
			}
			j++;
		}
	}

	if (Delarr.Num() != 0)
	{
		int i = 0;
		int delSize = Delarr.Num();

		while (i < PostBoxData.Num())
		{
			int index = Delarr[(delSize - 1)]->AsNumber();

			if (PostBoxData[i].EMIndex == index)
			{
				PostBoxData.RemoveAt(i);
				delSize = delSize - 1;
				i = 0;
				if (delSize == 0)
					break;

				continue;
			}

			i++;
		}
	}


}

// 우편함 del 이벤트 핸들링 
void AAOWGameModeBase::DeletePostBoxData(TSharedPtr<FJsonObject> JsonObject, FCustomStruct *event)
{
	event->Event = 19;
}

//우편함 read 이벤트 핸들링 
void AAOWGameModeBase::ReadPostBoxData(TSharedPtr<FJsonObject> JsonObject, FCustomStruct *event)
{
	event->Event = 20;
}

void AAOWGameModeBase::SetSimpleRank(TSharedPtr<FJsonObject> JsonObject, FCustomStruct *event)
{
	int RankTear = JsonObject->GetIntegerField("RankTear");
	int RankWin = JsonObject->GetIntegerField("RankWin");
	int RankTotal = JsonObject->GetIntegerField("RankTotal");
	
	event->Event = 21;
	event->RankTear = RankTear;
	event->RankWin = RankWin;
	event->RankTotal = RankTotal;

	UE_LOG(LogClass, Warning, TEXT("Login Check?"));

}

// total 랭크 처리 
void AAOWGameModeBase::SetTotalRank(TSharedPtr<FJsonObject> JsonObject, FCustomStruct *event)
{
	event->Event = 22;
	RankingData.Empty();
	TArray<TSharedPtr<FJsonValue>> TotalRankingArr = JsonObject->GetArrayField("RankingData");


	for (int i = 0; i < TotalRankingArr.Num(); i++) //배열에 인수가 몇개인지 알수있고 
	{
		FCustomStructRankingMSG msg;
		TArray<TSharedPtr<FJsonValue>> Data = TotalRankingArr[i]->AsArray();
		msg.RankNickName = Data[0]->AsString();
		msg.RankTear = Data[1]->AsNumber();
		msg.RankScore = Data[2]->AsNumber();
		msg.RankWinRank=i+1; // 순위 
		msg.RankhighLigth = 0;
	
		float win = Data[3]->AsNumber(); // 승리 횟수 
		float total = Data[4]->AsNumber();
		float winrate = (win / total)*100;
		msg.RankWinRate= winrate; // 승률 


		RankingData.Emplace(msg);
	}
	
}

// 내티어 찾기 
void AAOWGameModeBase::SetTearRank(TSharedPtr<FJsonObject> JsonObject, FCustomStruct *event)
{

}

// 구글 플레이 닉네임 등록 성공및 다음 작업 실행 
void AAOWGameModeBase::GooglePlayLoginSuccess(TSharedPtr<FJsonObject> JsonObject)
{
	FString NickName = JsonObject->GetStringField("NickName");
	//SESSION_Eamil = tmpEmail;
	SESSION_NickName = NickName;
	SESSION_Tear = 1;
}

// 구글 플레이 이미 등록된 로그인 
void AAOWGameModeBase::GooglePlayLoginRegisterSuccess(TSharedPtr<FJsonObject> JsonObject)
{
	FString NickName = JsonObject->GetStringField("NickName");
	FString GooleEmail = JsonObject->GetStringField("GoogleUID");
	int Tear = JsonObject->GetIntegerField("Tear");
	//SESSION_Eamil = tmpEmail;
	SESSION_NickName = NickName;
	SESSION_Tear = Tear;

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT(" ok resgister google !!!")));
}




// 로그인 요청 HTTP
void AAOWGameModeBase::httpLoginRequest()
{
	FString url = "http://ec2-52-79-216-45.ap-northeast-2.compute.amazonaws.com/Login/Login.php?";
	FString urltail = "Email=" + UserMail + "&Passwd=" + UserPasswd;
	url.Append(urltail);

	//UE_LOG(LogClass, Error, TEXT("Result ----------------> %s"), &url);

	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &AAOWGameModeBase::OnResponseReceived); // callback 함수 등록 

	////This is the url on which to process the request
	Request->SetURL(url);
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->ProcessRequest(); // 요청 함수 

	UE_LOG(LogClass, Error, TEXT("Result ----------------> %s --> %s"), &UserMail, &UserPasswd);

}

// 가입 요청 HTTP
void AAOWGameModeBase::httpSignUpRequest()
{
	FString url = "http://ec2-52-79-216-45.ap-northeast-2.compute.amazonaws.com/Login/SignUp.php?";
	FString urltail = "NickName=" + SignNickName + "&Email=" + SignMail + "&Passwd=" + SignPasswd;
	url.Append(urltail);

	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &AAOWGameModeBase::OnResponseReceived); // callback 함수 등록 

	//This is the url on which to process the request
	Request->SetURL(url);
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->ProcessRequest(); // 요청 함수 

}

// postsize요청
void AAOWGameModeBase::httpGetPostBoxSize()
{
	FString url = "http://ec2-52-79-216-45.ap-northeast-2.compute.amazonaws.com/PostBox/getPostSize.php?";
	FString urltail = "NickName=" + SESSION_NickName;
	url.Append(urltail);

	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &AAOWGameModeBase::OnResponseReceived); // callback 함수 등록 

	//This is the url on which to process the request
	Request->SetURL(url);
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->ProcessRequest(); // 요청 함수 


}

void AAOWGameModeBase::http_GetPostBoxData()
{

	FString url = "http://ec2-52-79-216-45.ap-northeast-2.compute.amazonaws.com/PostBox/getPostBox.php?";
	FString urltail = "NickName=" + SESSION_NickName;
	url.Append(urltail);

	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &AAOWGameModeBase::OnResponseReceived); // callback 함수 등록 

	//This is the url on which to process the request
	Request->SetURL(url);
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->ProcessRequest(); // 요청 함수 

}
//post box delete 요청 
void AAOWGameModeBase::http_DeletePostBoxData(int EMIndex)
{
	FString EMindex = FString::FromInt(EMIndex);
	FString url = "http://ec2-52-79-216-45.ap-northeast-2.compute.amazonaws.com/PostBox/DelPostBox.php?";
	FString urltail = "NickName=" + SESSION_NickName + "&EMindex=" + EMindex;
	url.Append(urltail);

	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &AAOWGameModeBase::OnResponseReceived); // callback 함수 등록 

	//This is the url on which to process the request
	Request->SetURL(url);
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->ProcessRequest(); // 요청 함수 

}

// post box read 요청 
void AAOWGameModeBase::http_ReadPostBoxData(int EMIndex)
{
	FString EMindex = FString::FromInt(EMIndex);
	FString url = "http://ec2-52-79-216-45.ap-northeast-2.compute.amazonaws.com/PostBox/ReadPostBox.php?";
	FString urltail = "NickName=" + SESSION_NickName + "&EMindex=" + EMindex;
	url.Append(urltail);

	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &AAOWGameModeBase::OnResponseReceived); // callback 함수 등록 

	//This is the url on which to process the request
	Request->SetURL(url);
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->ProcessRequest(); // 요청 함수 

}

// 매칭 심플 ui 리퀘스트
void AAOWGameModeBase::http_RequestSimpleRank() 
{
	FString url = "http://ec2-52-79-216-45.ap-northeast-2.compute.amazonaws.com/Ranking/GetSimpleRank.php?";
	FString urltail = "NickName=" + SESSION_NickName;
	url.Append(urltail);

	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &AAOWGameModeBase::OnResponseReceived); // callback 함수 등록 

	//This is the url on which to process the request
	Request->SetURL(url);
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->ProcessRequest(); // 요청 함수 


}

// 전체랭킹 순위 조회 
void AAOWGameModeBase::http_RequestTotalRank(int nowTear) 
{
	FString url = "http://ec2-52-79-216-45.ap-northeast-2.compute.amazonaws.com/Ranking/GetTotalRank.php";
	
	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &AAOWGameModeBase::OnResponseReceived); // callback 함수 등록 

	//This is the url on which to process the request
	Request->SetURL(url);
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->ProcessRequest(); // 요청 함수 
}

// 티어 랭킹 순위 조회 
void AAOWGameModeBase::http_RequestTearRank(int nowTear)
{
	FString EMindex = FString::FromInt(nowTear);
	FString url = "http://ec2-52-79-216-45.ap-northeast-2.compute.amazonaws.com/Ranking/GetTearRank.php?";
	FString urltail = "NowTear=" + EMindex;
	url.Append(urltail);

	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &AAOWGameModeBase::OnResponseReceived); // callback 함수 등록 

	//This is the url on which to process the request
	Request->SetURL(url);
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->ProcessRequest(); // 요청 함수 

}

//GET Google id
FString AAOWGameModeBase::GetGooleoPlayUniqueID(APlayerController* PlayerController)
{
	FString UniqueID = "Fail!!";

	if (PlayerController && PlayerController->PlayerState && PlayerController->PlayerState->UniqueId.IsValid())
	{
		UniqueID = PlayerController->PlayerState->UniqueId->GetHexEncodedString();
	}

	//FString EMindex = FString::FromInt(nowTear);
	FString url = "http://ec2-52-79-216-45.ap-northeast-2.compute.amazonaws.com/Login/GoogleLogin.php?";
	FString urltail = "NetUID=" + UniqueID;
	url.Append(urltail);

	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &AAOWGameModeBase::OnResponseReceived); // callback 함수 등록 이건 좀더 다른 방향으로 수정합시다 

	//This is the url on which to process the request
	Request->SetURL(url);
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->ProcessRequest(); // 요청 함수 

	return UniqueID;
}

//구글 플레이 닉네임 등록 
void AAOWGameModeBase::http_GooleoPlayNameSend(FString name, const FString NetUID)
{
	FString url = "http://ec2-52-79-216-45.ap-northeast-2.compute.amazonaws.com/Login/GoogleAddName.php?";
	FString urltail = "NickName=" + name + "&NetUID=" + NetUID;
	url.Append(urltail);

	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &AAOWGameModeBase::OnResponseReceived); // callback 함수 등록 이건 좀더 다른 방향으로 수정합시다 

	//This is the url on which to process the request
	Request->SetURL(url);
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->ProcessRequest(); // 요청 함수 
}



// http callback 핸들러
void AAOWGameModeBase::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{

	TSharedPtr<FJsonObject> JsonObject; //Create a pointer to hold the json serialized data
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());// JSON 데이터 변수 선언 
	int RequestHandle = 0; // 100 회원가입
	FCustomStruct event;

	//json 시러얼라이즈 해제 
	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		FString tmpFlag = JsonObject->GetStringField("HandleFlag");
		RequestHandle = FCString::Atoi(*tmpFlag);

		switch (RequestHandle)
		{
		case 100: // 회원가입 핸들 
			SignUpCheck(JsonObject);
			break;

		case 200: // 로그인 핸들 
			LoginCheck(JsonObject);
			break;

		case 300: // PostBox 핸들링 메세지 알림용 -> ! <- 알림
			httpHandleGetPostBoxSize(JsonObject, &event);
			EventHandlerData.Add(event);
			break;

		case 310: // getPostBox data 핸들링  
			GetPostBoxDataHandle(JsonObject, &event);
			EventHandlerData.Add(event);
			break;

		case 320: // Delete,Read postbox
			http_GetPostBoxData();
			DeletePostBoxData(JsonObject, &event);
			EventHandlerData.Add(event);
			break;

		case 400:
			SetSimpleRank(JsonObject, &event);
			EventHandlerData.Add(event);
			break;

		case 410: // 티어랭킹, 전체랭킹 리스트뷰 처리 
			SetTotalRank(JsonObject, &event);
			EventHandlerData.Add(event);
			break;

		case 500: // 구글 플레이 로그인에 등록되지않은 사용자 처리
			LoginCheckFlag = 4;
			break;

		case 510: // 구글 플레이 닉네임 등록 처리 및 매칭 화면 이동 
			GooglePlayLoginSuccess(JsonObject);
			LoginCheckFlag = 5;
			break;

		case 520: // 구글 플레이 닉네임 등록 닉네임 중복 
			LoginCheckFlag = 6; 
			break;

		case 530: // 구글 플레이 아이디 등록된 아이디 
			GooglePlayLoginRegisterSuccess(JsonObject);
			LoginCheckFlag = 5;
			break;

		default:
			break;
		}

	}
}



/////////////////////////////////////////// MATCHING LOGIC ///////////////////////////////////////////////

bool AAOWGameModeBase::ConnMatching()
{
	if (!MatChginConnCreateSocket("conn_MatchingServer", "115.71.232.4", 9887)) //연결 소켓 제작
	{
		return false;
	}
	return true;
}

//matching connection 
bool AAOWGameModeBase::MatChginConnCreateSocket(const FString& YourChosenSocketName, const FString& TheIP, const int32 ThePort)
{
	FIPv4Address Addr;
	int32 port = 9887;
	AAOWGameModeBase::Parse(TheIP, Addr);

	ConnectionSocket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("ClientSocket1"), false); // 연결할 소켓 만들기 
	TSharedRef<FInternetAddr> RemoteAddress = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr(); // 주소 생성

	RemoteAddress->SetIp(Addr.Value);
	RemoteAddress->SetPort(port);

	bool connected = ConnectionSocket->Connect(*RemoteAddress); //소켓 연결 

	if (!connected)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connect fail.")));
		return false;
	}

	UWorld* World = GetWorld();
	World->GetTimerManager().SetTimer(TCPSocketListenerTimerHandle, this, &AAOWGameModeBase::TCPSocketListener, 0.5f, true); // socket 리시버

	return true;
}

//바이너리 데이터 string으로 포맷팅 
FString AAOWGameModeBase::StringFromBinaryArray(TArray<uint8> BinaryArray)
{
	//Create a string from a byte array!s
	const std::string cstr(reinterpret_cast<const char*>(BinaryArray.GetData()), BinaryArray.Num());
	return FString(cstr.c_str());

}

//이번에 새로 만든 형태 
bool AAOWGameModeBase::SendMessage(const FString& Message)
{
	if (ConnectionSocket)
	{
		TArray<uint8> Payload;
		BinaryArrayFromString(Message, Payload);
		FSocketMessageHeader::WrapAndSendPayload(Payload, ConnectionSocket);
		FString recvmsg = StringFromBinaryArray(Payload);
		return true;
	}
	return false;
}

// 문자열 바이너리 변형
void AAOWGameModeBase::BinaryArrayFromString(const FString& Message, TArray<uint8>& OutBinaryArray)
{
	FTCHARToUTF8 Convert(*Message);
	OutBinaryArray.Empty();
	OutBinaryArray.Append((UTF8CHAR*)Convert.Get(), Convert.Length());
}

//tcp recv
void AAOWGameModeBase::TCPSocketListener()
{
	if (!ConnectionSocket) //소켓conn?
	{
		UE_LOG(LogClass, Warning, TEXT("Conn not!!!"));
		return;
	}

	//Binary Array!
	TArray<uint8> ReceivedData;
	uint32 Size;

	while (ConnectionSocket->HasPendingData(Size))
	{
		int32 Read = 0;
		ReceivedData.Init(FMath::Min(Size, 65507u), Size);
		ConnectionSocket->Recv(ReceivedData.GetData(), ReceivedData.Num(), Read); //블로킹 아님 
	}

	if (ReceivedData.Num() <= 0)
	{
		return;
	}

	//read
	FString recvmsg = StringFromBinaryArray(ReceivedData);
	TArray<FString> Array = {};
	recvmsg.ParseIntoArray(Array, TEXT("?"));
	for (int32 ArrayNum = 0; ArrayNum < Array.Num(); ++ArrayNum)
	{
		UE_LOG(LogClass, Warning, TEXT("Array : %s"), *Array[ArrayNum]);
		TSharedPtr<FJsonObject> JsonObject; //Create a pointer to hold the json serialized data
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(*Array[ArrayNum]);
		FJsonSerializer::Deserialize(Reader, JsonObject);
		recvMatchEventHandle(JsonObject);
	}



}

// recv 매칭 이벤트 핸들러
void AAOWGameModeBase::recvMatchEventHandle(TSharedPtr<FJsonObject> JsonObject)
{

	FCustomStruct event;
	int eventFlag = JsonObject->GetIntegerField(TEXT("Event"));

	switch (eventFlag)
	{
	case 10: // 매칭서버연결성공, 유저 기본 정보 등록
		SendMessage(create_msg_event10());
		break;
	case 11: //2차매칭 전환 이벤트 
		event.Event = 11;
		EventHandlerData.Add(event);
		break;
	case 12: //2차매칭 캔슬 및 수락 UI 변경  
		LastMatchingEventHandling(JsonObject, &event);
		EventHandlerData.Add(event);
		break;
	case 13: // 메세지 셋팅? 
		event.Event = 13;
		EventHandlerData.Add(event);
		break;
	case 15: // 2차 매칭 타임 아웃 
		event.Event = 15;
		EventHandlerData.Add(event);
		break;

	case 20: // 매칭 이후 게임 시작 핸들링  
		setGameHandling(JsonObject, &event);
		EventHandlerData.Add(event);
		break;

	}

}

FString AAOWGameModeBase::create_msg_event10()
{
	FString tmpSessionID = SessionID.ToString(); // 이미 설정되어있는 같은값 
	FString OutputString;

	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	JsonObject->SetStringField("SessionID", tmpSessionID);
	JsonObject->SetStringField("SessionNickName", SESSION_NickName);
	JsonObject->SetStringField("SessionEmail", SESSION_Eamil);
	JsonObject->SetNumberField("SessionTear", SESSION_Tear);
	JsonObject->SetStringField("SessionMac", SESSION_UserMacID);
	JsonObject->SetNumberField("Event", 10);

	TSharedRef< TJsonWriter<> > Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	return OutputString;
	// 이렇게 직렬화 가능
	//TSharedRef< TJsonWriter<TCHAR, TPrettyJsonPrintPolicy<TCHAR> > > JsonWriter = TJsonWriterFactory<TCHAR, TPrettyJsonPrintPolicy<TCHAR>>::Create(&OutputString2);
	//FJsonSerializer::Serialize(JsonObject.ToSharedRef(), JsonWriter);

}

void AAOWGameModeBase::startMatching() {

	FString OutputString;
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	JsonObject->SetNumberField("Event", 20);
	JsonObject->SetStringField("SessionID", SessionID.ToString());
	JsonObject->SetStringField("SessionMac", SESSION_UserMacID);

	TSharedRef< TJsonWriter<> > Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
	SendMessage(OutputString);
}

void AAOWGameModeBase::CancleMatch() {

	FString OutputString;
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	JsonObject->SetNumberField("Event", 30);
	JsonObject->SetStringField("SessionID", SessionID.ToString());
	JsonObject->SetStringField("SessionMac", SESSION_UserMacID);

	TSharedRef< TJsonWriter<> > Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
	SendMessage(OutputString);
}

void AAOWGameModeBase::LastMatchingEventHandling(TSharedPtr<FJsonObject> JsonObject, FCustomStruct* event)
{
	int LastMatchingType = JsonObject->GetIntegerField(TEXT("LastMatchingType"));
	event->LastMatchingType = LastMatchingType;
	// 수락 
	if (LastMatchingType == 1)
		LastSuccessEventProcess(JsonObject, event);

	//캔슬
	if (LastMatchingType == 2)
	{
		LastCancleEventProcess(JsonObject, event);
		time_t start = time(NULL);
		FTimerDelegate LastMatchingDelegate;
		LastMatchingDelegate.BindUFunction(this, FName("LastMatchingResetUI"), (float)start); // 조금있다가 자동으로 2차매칭 ui 창 리셋 시키는이벤트 
		GetWorldTimerManager().SetTimer(LastMatchingTimerHandle, LastMatchingDelegate, 2.f, true);
	}
}

void AAOWGameModeBase::LastCancleMatch()
{
	FString OutputString;
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	JsonObject->SetNumberField("Event", 40);
	JsonObject->SetStringField("SessionID", SessionID.ToString());
	JsonObject->SetStringField("SessionMac", SESSION_UserMacID);
	JsonObject->SetNumberField("RequestOwnerType", 1);

	TSharedRef< TJsonWriter<> > Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
	SendMessage(OutputString);
}

void AAOWGameModeBase::LastCancleEventProcess(TSharedPtr<FJsonObject> JsonObject, FCustomStruct* event)
{
	int RequestType = JsonObject->GetIntegerField(TEXT("RequestOwnerType"));
	event->RequestOwnerType = RequestType;
	event->Event = 12;
}

void AAOWGameModeBase::LastSuccessMatch()
{
	FString OutputString;
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	JsonObject->SetNumberField("Event", 50);
	JsonObject->SetStringField("SessionID", SessionID.ToString());
	JsonObject->SetStringField("SessionMac", SESSION_UserMacID);
	JsonObject->SetNumberField("RequestOwnerType", 1);

	TSharedRef< TJsonWriter<> > Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
	SendMessage(OutputString);

}

void AAOWGameModeBase::LastSuccessEventProcess(TSharedPtr<FJsonObject> JsonObject, FCustomStruct* event)
{
	int RequestType = JsonObject->GetIntegerField(TEXT("RequestOwnerType"));
	event->RequestOwnerType = RequestType;
	event->Event = 12;
}

FCustomStruct AAOWGameModeBase::getEventData()
{
	FCustomStruct data = EventHandlerData[0];
	return data;
}

int AAOWGameModeBase::getEventHandlerSize()
{
	return EventHandlerData.Num();
}

void AAOWGameModeBase::removeEventHandlerData()
{
	EventHandlerData.RemoveAt(0);
}

void AAOWGameModeBase::CountDownThreadRun()
{
	CountDownValue = 10; // 카운트 값 초기화 
	threadmanager = NewObject<UThreadTaskManager>(this);
	threadmanager->CountDownStartProcess(&CountDownValue);
}

void AAOWGameModeBase::LastMatchingResetUI(float start)
{
	FCustomStruct event;
	event.Event = 14;
	time_t end;
	float result = 0;
	end = time(NULL);
	result = (float)(end - start);

	if (result <= 4.1)
	{
		GetWorld()->GetTimerManager().ClearTimer(LastMatchingTimerHandle);
		EventHandlerData.Add(event);
	}
}

void AAOWGameModeBase::setGameHandling(TSharedPtr<FJsonObject> JsonObject, FCustomStruct* event)
{

	GetWorld()->GetTimerManager().ClearTimer(TCPSocketListenerTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(Chat_TCPSocketListenerTimerHandle);
	
	ConnectionSocket->Close();
	Chat_ConnectionSocket->Close();

	FString matchingIndex = JsonObject->GetStringField("MatchingIndex");
	int userType = JsonObject->GetNumberField("UserType");

	gameInstance->fsGameOption.game_sessionid = SessionID.ToString();
	gameInstance->fsGameOption.game_macid = SESSION_UserMacID;
	gameInstance->fsGameOption.game_usernickname = SESSION_NickName;
	gameInstance->fsGameOption.game_gameindex = matchingIndex;
	gameInstance->fsGameOption.userType = userType;
	
	UE_LOG(LogClass, Warning, TEXT("Set fsGameOption!!"));
	event->Event = 20;
}

/////////////////////////////////////////// Chatting LOGIC ///////////////////////////////////////////////

bool AAOWGameModeBase::ConnChatting()
{
	if (!Chatting_ConnCreateSocket("Chatting_Server", "52.79.216.45", 9777))
	{
		return false;
	}
	return true;
}

bool AAOWGameModeBase::Chatting_ConnCreateSocket(const FString& YourChosenSocketName, const FString& TheIP, const int32 ThePort)
{
	FIPv4Address Addr;
	int32 port = ThePort;
	AAOWGameModeBase::Parse(TheIP, Addr);

	Chat_ConnectionSocket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("ChatClientSocket"), false); // 연결할 소켓 만들기 
	TSharedRef<FInternetAddr> RemoteAddress = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr(); // 주소 생성

	RemoteAddress->SetIp(Addr.Value);
	RemoteAddress->SetPort(port);

	bool connected = Chat_ConnectionSocket->Connect(*RemoteAddress); //소켓 연결 

	if (!connected)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connect fail.")));
		return false;
	}

	UWorld* World = GetWorld();
	World->GetTimerManager().SetTimer(Chat_TCPSocketListenerTimerHandle, this, &AAOWGameModeBase::Chatting_TCPSocketListener, 0.5f, true); // socket 리시버

	return true;

}

void AAOWGameModeBase::Chatting_TCPSocketListener()
{
	if (!Chat_ConnectionSocket) //소켓conn?
	{
		return;
	}

	//Binary Array!
	TArray<uint8> ReceivedData;
	uint32 Size;

	while (Chat_ConnectionSocket->HasPendingData(Size))
	{
		int32 Read = 0;
		ReceivedData.Init(FMath::Min(Size, 65507u), Size);
		Chat_ConnectionSocket->Recv(ReceivedData.GetData(), ReceivedData.Num(), Read); //블로킹 아님 

	}

	if (ReceivedData.Num() <= 0) {
		return;
	}

	//read
	FString recvmsg = StringFromBinaryArray(ReceivedData);
	TSharedPtr<FJsonObject> JsonObject; //Create a pointer to hold the json serialized data
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(recvmsg);
	FJsonSerializer::Deserialize(Reader, JsonObject);
	ChattingrecvEventHandle(JsonObject); // 여기서 문제생김 -> 무슨 문제? 게임 승리하고 다시 접속했을때 메세지를 못읽는? 그런거같음 
}


void AAOWGameModeBase::ChattingrecvEventHandle(TSharedPtr<FJsonObject> JsonObject)
{
	FCustomStruct event;
	int eventFlag = JsonObject->GetIntegerField(TEXT("Event"));

	switch (eventFlag) // 14 사용 가능 16부터 사용 가능 
	{
	case 50: // 채팅서버연결성공, 유저 기본 정보 등록
		Chatting_SendUserInfo(JsonObject);
		break;

	case 51: // 채팅 채널 첫입장시 유저 목록 동기화 
		firstChannelUserList(JsonObject);
		break;

	case 60: // 채널별 인원수 동기화 
		setChannelListSize(JsonObject);
		break;

	case 70: // 채널 이동 처리
		setChannelUserList(JsonObject, &event);
		EventHandlerData.Add(event);
		break;

	case 71: // 첫 채널 입장시 채널 ui및 데이터 처리  
		break;

	case 80: // 임시 채팅 메세지 
		setChattingMsg(JsonObject, &event);
		EventHandlerData.Add(event);
		break;

	case 100: // 긴급 메세지 처리 
		EmergencyMsg(JsonObject, &event);
		EventHandlerData.Add(event);
		break;
	}
}

void AAOWGameModeBase::setChattingMsg(TSharedPtr<FJsonObject> JsonObject, FCustomStruct *event)
{
	event->Event = 16;
	event->sessionID = JsonObject->GetStringField("SenderNickName");
	event->msg = JsonObject->GetStringField("MSG");
}


bool AAOWGameModeBase::Chatting_SendMessage(const FString& Message)
{
	if (Chat_ConnectionSocket)
	{
		TArray<uint8> Payload;
		BinaryArrayFromString(Message, Payload);
		FSocketMessageHeader::WrapAndSendPayload(Payload, Chat_ConnectionSocket);
		FString recvmsg = StringFromBinaryArray(Payload);
		return true;
	}
	return false;
}

//  chatting 메세지 send
void AAOWGameModeBase::Chatting_sendmsg(FString msg)
{
	ConnectionSocket->Close();

	FString OutputString;
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	JsonObject->SetStringField("MSG", msg);
	JsonObject->SetStringField("SenderNickName", SESSION_NickName);
	JsonObject->SetNumberField("Event", 50);
	JsonObject->SetNumberField("ChannelNum", nowChannelNum);
	TSharedRef< TJsonWriter<> > Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
	Chatting_SendMessage(OutputString);
}

void  AAOWGameModeBase::Chatting_SendUserInfo(TSharedPtr<FJsonObject> JsonObject)
{
	int RoomNum = JsonObject->GetIntegerField(TEXT("ChannelNum"));
	nowChannelNum = RoomNum; //현재 채널 인원 셋팅 
	UE_LOG(LogClass, Warning, TEXT("now chaanel-> %d"), nowChannelNum);
	FString tmpSessionID = SessionID.ToString(); // 세션 아이디 체크가 중요할듯 
	FString OutputString;

	TSharedPtr<FJsonObject> JsonObject_ = MakeShareable(new FJsonObject);
	JsonObject_->SetStringField("SessionNickName", SESSION_NickName);
	JsonObject_->SetStringField("SessionID", tmpSessionID);
	JsonObject_->SetNumberField("Event", 20);
	JsonObject_->SetNumberField("ChannelNum", RoomNum);

	TSharedRef< TJsonWriter<> > Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObject_.ToSharedRef(), Writer);
	Chatting_SendMessage(OutputString);
}


void AAOWGameModeBase::setChannelListSize(TSharedPtr<FJsonObject> JsonObject)
{
	TArray<TSharedPtr<FJsonValue>> arr = JsonObject->GetArrayField("ChannelListSize");

	for (int i = 0; i < arr.Num(); i++) //배열에 인수가 몇개인지 알수있고 
	{
		int num = arr[i]->AsNumber();
		channellist.Emplace(num); //채널 유저 데이터 셋팅 
	}
	ChannelListCheck = false;
}


TArray<int>& AAOWGameModeBase::getChannelList()
{
	ChannelListCheck = true; // 초기값으로 돌려놓음 
	return channellist;
}


void AAOWGameModeBase::Chatting_getChannelList()
{
	ChannelListCheck = true;
	FString OutputString;
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	JsonObject->SetNumberField("Event", 30);
	TSharedRef< TJsonWriter<> > Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
	Chatting_SendMessage(OutputString);

}

void AAOWGameModeBase::removeChannelList()
{
	channellist.Empty();
}

void AAOWGameModeBase::moveChannel(int ChannelNum)
{
	FString OutputString;
	FString tmpSessionID = SessionID.ToString();
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	JsonObject->SetNumberField("Event", 40);
	JsonObject->SetNumberField("ChannelNum", ChannelNum);
	JsonObject->SetNumberField("PreviousChannelNum", nowChannelNum);
	JsonObject->SetStringField("SessionID", tmpSessionID);
	TSharedRef< TJsonWriter<> > Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
	Chatting_SendMessage(OutputString);

}

void AAOWGameModeBase::firstChannelUserList(TSharedPtr<FJsonObject> JsonObject)
{
	int RoomNum = JsonObject->GetIntegerField("ChannelNum");
	FString OutputString;
	TSharedPtr<FJsonObject> JsonObject2 = MakeShareable(new FJsonObject);
	JsonObject2->SetNumberField("Event", 41);
	JsonObject2->SetNumberField("ChannelNum", RoomNum);
	TSharedRef< TJsonWriter<> > Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObject2.ToSharedRef(), Writer);
	Chatting_SendMessage(OutputString);
}

void AAOWGameModeBase::setChannelUserList(TSharedPtr<FJsonObject> JsonObject, FCustomStruct *event)
{
	event->Event = 13;
	event->CheckFlag = JsonObject->GetIntegerField("CheckFlag");
	event->ChannelNum = JsonObject->GetIntegerField("ChannelNum"); //채널 셋팅 
	nowChannelNum = JsonObject->GetIntegerField("ChannelNum"); // 변경된 채널 셋팅
	TArray<TSharedPtr<FJsonValue>> arr = JsonObject->GetArrayField("ChannelUserList"); // 채널 유저 목록 

	for (int i = 0; i < arr.Num(); i++) //데이터 셋팅하는거 까지 완료 
	{
		FString data = arr[i]->AsString();
		event->ChannelUserList.Emplace(data);

	}
}

void AAOWGameModeBase::EmergencyMsg(TSharedPtr<FJsonObject> JsonObject, FCustomStruct *event)
{
	FString EmergencyMsg = JsonObject->GetStringField(TEXT("EmergencyMsg"));
	event->msg = EmergencyMsg;
	event->Event = 10;
}

// 랭킹 하이라이트 체인지 함수 
void AAOWGameModeBase::RankingHighlightChange(int index) 
{
	RankingData[index].RankhighLigth = 1;
}