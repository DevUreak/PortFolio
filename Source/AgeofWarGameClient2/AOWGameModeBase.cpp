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


static FGuid SessionID = FGuid::NewGuid(); //���� ���̵� ���
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
	nowChannelNum = -1; // ä�� ���� 
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
	GetWorld()->GetTimerManager().ClearTimer(TCPSocketListenerTimerHandle); //recv �ڵ鷯 ���� 
	GetWorld()->GetTimerManager().ClearTimer(Chat_TCPSocketListenerTimerHandle);

	if (ConnectionSocket != NULL) { //��� ���� ����
		ConnectionSocket->Close();
		UE_LOG(LogClass, Warning, TEXT("Call Close CONNSOCKET!!!!"));
	}

}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////HTTP CODE


//ȸ�� ���� Ȯ�� �޼ҵ� 
void AAOWGameModeBase::SignUpCheck(TSharedPtr<FJsonObject> JsonObject) {

	FString tmp = JsonObject->GetStringField("SignUpCheck");
	SignUpCheckFlag = FCString::Atoi(*tmp);
}
//�α��� Ȯ�� �޼ҵ� 
void AAOWGameModeBase::LoginCheck(TSharedPtr<FJsonObject> JsonObject) {

	FString tmpFlag = JsonObject->GetStringField("LoginCheck");
	FString tmpEmail;
	FString tmpNickName;
	int tmpTear;
	LoginCheckFlag = FCString::Atoi(*tmpFlag);

	if (LoginCheckFlag == 2) // �α��μ��� // ���� ���� ���� 
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

//������ size setting 
void AAOWGameModeBase::httpHandleGetPostBoxSize(TSharedPtr<FJsonObject> JsonObject, FCustomStruct *event)
{
	int PostSize = JsonObject->GetIntegerField("PostSize");
	event->Event = 17;
	event->PostBoxSize = PostSize;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("size-> %d"), PostSize));
}

//������ set ������ �ڵ鸵
void AAOWGameModeBase::GetPostBoxDataHandle(TSharedPtr<FJsonObject> JsonObject, FCustomStruct *event)
{
	event->Event = 18;
	PostBoxData.Empty();
	TArray<TSharedPtr<FJsonValue>> Delarr = JsonObject->GetArrayField("DeleteArray");
	TArray<TSharedPtr<FJsonValue>> Readarr = JsonObject->GetArrayField("ReadArray");
	TArray<TSharedPtr<FJsonValue>> Datarr = JsonObject->GetArrayField("PostData");


	for (int i = 0; i < Datarr.Num(); i++) //�迭�� �μ��� ����� �˼��ְ� 
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

			if (PostBoxData[j].EMIndex == index) // index�� ���ٸ� 
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

// ������ del �̺�Ʈ �ڵ鸵 
void AAOWGameModeBase::DeletePostBoxData(TSharedPtr<FJsonObject> JsonObject, FCustomStruct *event)
{
	event->Event = 19;
}

//������ read �̺�Ʈ �ڵ鸵 
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

// total ��ũ ó�� 
void AAOWGameModeBase::SetTotalRank(TSharedPtr<FJsonObject> JsonObject, FCustomStruct *event)
{
	event->Event = 22;
	RankingData.Empty();
	TArray<TSharedPtr<FJsonValue>> TotalRankingArr = JsonObject->GetArrayField("RankingData");


	for (int i = 0; i < TotalRankingArr.Num(); i++) //�迭�� �μ��� ����� �˼��ְ� 
	{
		FCustomStructRankingMSG msg;
		TArray<TSharedPtr<FJsonValue>> Data = TotalRankingArr[i]->AsArray();
		msg.RankNickName = Data[0]->AsString();
		msg.RankTear = Data[1]->AsNumber();
		msg.RankScore = Data[2]->AsNumber();
		msg.RankWinRank=i+1; // ���� 
		msg.RankhighLigth = 0;
	
		float win = Data[3]->AsNumber(); // �¸� Ƚ�� 
		float total = Data[4]->AsNumber();
		float winrate = (win / total)*100;
		msg.RankWinRate= winrate; // �·� 


		RankingData.Emplace(msg);
	}
	
}

// ��Ƽ�� ã�� 
void AAOWGameModeBase::SetTearRank(TSharedPtr<FJsonObject> JsonObject, FCustomStruct *event)
{

}

// ���� �÷��� �г��� ��� ������ ���� �۾� ���� 
void AAOWGameModeBase::GooglePlayLoginSuccess(TSharedPtr<FJsonObject> JsonObject)
{
	FString NickName = JsonObject->GetStringField("NickName");
	//SESSION_Eamil = tmpEmail;
	SESSION_NickName = NickName;
	SESSION_Tear = 1;
}

// ���� �÷��� �̹� ��ϵ� �α��� 
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




// �α��� ��û HTTP
void AAOWGameModeBase::httpLoginRequest()
{
	FString url = "http://ec2-52-79-216-45.ap-northeast-2.compute.amazonaws.com/Login/Login.php?";
	FString urltail = "Email=" + UserMail + "&Passwd=" + UserPasswd;
	url.Append(urltail);

	//UE_LOG(LogClass, Error, TEXT("Result ----------------> %s"), &url);

	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &AAOWGameModeBase::OnResponseReceived); // callback �Լ� ��� 

	////This is the url on which to process the request
	Request->SetURL(url);
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->ProcessRequest(); // ��û �Լ� 

	UE_LOG(LogClass, Error, TEXT("Result ----------------> %s --> %s"), &UserMail, &UserPasswd);

}

// ���� ��û HTTP
void AAOWGameModeBase::httpSignUpRequest()
{
	FString url = "http://ec2-52-79-216-45.ap-northeast-2.compute.amazonaws.com/Login/SignUp.php?";
	FString urltail = "NickName=" + SignNickName + "&Email=" + SignMail + "&Passwd=" + SignPasswd;
	url.Append(urltail);

	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &AAOWGameModeBase::OnResponseReceived); // callback �Լ� ��� 

	//This is the url on which to process the request
	Request->SetURL(url);
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->ProcessRequest(); // ��û �Լ� 

}

// postsize��û
void AAOWGameModeBase::httpGetPostBoxSize()
{
	FString url = "http://ec2-52-79-216-45.ap-northeast-2.compute.amazonaws.com/PostBox/getPostSize.php?";
	FString urltail = "NickName=" + SESSION_NickName;
	url.Append(urltail);

	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &AAOWGameModeBase::OnResponseReceived); // callback �Լ� ��� 

	//This is the url on which to process the request
	Request->SetURL(url);
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->ProcessRequest(); // ��û �Լ� 


}

void AAOWGameModeBase::http_GetPostBoxData()
{

	FString url = "http://ec2-52-79-216-45.ap-northeast-2.compute.amazonaws.com/PostBox/getPostBox.php?";
	FString urltail = "NickName=" + SESSION_NickName;
	url.Append(urltail);

	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &AAOWGameModeBase::OnResponseReceived); // callback �Լ� ��� 

	//This is the url on which to process the request
	Request->SetURL(url);
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->ProcessRequest(); // ��û �Լ� 

}
//post box delete ��û 
void AAOWGameModeBase::http_DeletePostBoxData(int EMIndex)
{
	FString EMindex = FString::FromInt(EMIndex);
	FString url = "http://ec2-52-79-216-45.ap-northeast-2.compute.amazonaws.com/PostBox/DelPostBox.php?";
	FString urltail = "NickName=" + SESSION_NickName + "&EMindex=" + EMindex;
	url.Append(urltail);

	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &AAOWGameModeBase::OnResponseReceived); // callback �Լ� ��� 

	//This is the url on which to process the request
	Request->SetURL(url);
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->ProcessRequest(); // ��û �Լ� 

}

// post box read ��û 
void AAOWGameModeBase::http_ReadPostBoxData(int EMIndex)
{
	FString EMindex = FString::FromInt(EMIndex);
	FString url = "http://ec2-52-79-216-45.ap-northeast-2.compute.amazonaws.com/PostBox/ReadPostBox.php?";
	FString urltail = "NickName=" + SESSION_NickName + "&EMindex=" + EMindex;
	url.Append(urltail);

	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &AAOWGameModeBase::OnResponseReceived); // callback �Լ� ��� 

	//This is the url on which to process the request
	Request->SetURL(url);
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->ProcessRequest(); // ��û �Լ� 

}

// ��Ī ���� ui ������Ʈ
void AAOWGameModeBase::http_RequestSimpleRank() 
{
	FString url = "http://ec2-52-79-216-45.ap-northeast-2.compute.amazonaws.com/Ranking/GetSimpleRank.php?";
	FString urltail = "NickName=" + SESSION_NickName;
	url.Append(urltail);

	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &AAOWGameModeBase::OnResponseReceived); // callback �Լ� ��� 

	//This is the url on which to process the request
	Request->SetURL(url);
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->ProcessRequest(); // ��û �Լ� 


}

// ��ü��ŷ ���� ��ȸ 
void AAOWGameModeBase::http_RequestTotalRank(int nowTear) 
{
	FString url = "http://ec2-52-79-216-45.ap-northeast-2.compute.amazonaws.com/Ranking/GetTotalRank.php";
	
	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &AAOWGameModeBase::OnResponseReceived); // callback �Լ� ��� 

	//This is the url on which to process the request
	Request->SetURL(url);
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->ProcessRequest(); // ��û �Լ� 
}

// Ƽ�� ��ŷ ���� ��ȸ 
void AAOWGameModeBase::http_RequestTearRank(int nowTear)
{
	FString EMindex = FString::FromInt(nowTear);
	FString url = "http://ec2-52-79-216-45.ap-northeast-2.compute.amazonaws.com/Ranking/GetTearRank.php?";
	FString urltail = "NowTear=" + EMindex;
	url.Append(urltail);

	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &AAOWGameModeBase::OnResponseReceived); // callback �Լ� ��� 

	//This is the url on which to process the request
	Request->SetURL(url);
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->ProcessRequest(); // ��û �Լ� 

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
	Request->OnProcessRequestComplete().BindUObject(this, &AAOWGameModeBase::OnResponseReceived); // callback �Լ� ��� �̰� ���� �ٸ� �������� �����սô� 

	//This is the url on which to process the request
	Request->SetURL(url);
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->ProcessRequest(); // ��û �Լ� 

	return UniqueID;
}

//���� �÷��� �г��� ��� 
void AAOWGameModeBase::http_GooleoPlayNameSend(FString name, const FString NetUID)
{
	FString url = "http://ec2-52-79-216-45.ap-northeast-2.compute.amazonaws.com/Login/GoogleAddName.php?";
	FString urltail = "NickName=" + name + "&NetUID=" + NetUID;
	url.Append(urltail);

	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &AAOWGameModeBase::OnResponseReceived); // callback �Լ� ��� �̰� ���� �ٸ� �������� �����սô� 

	//This is the url on which to process the request
	Request->SetURL(url);
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->ProcessRequest(); // ��û �Լ� 
}



// http callback �ڵ鷯
void AAOWGameModeBase::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{

	TSharedPtr<FJsonObject> JsonObject; //Create a pointer to hold the json serialized data
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());// JSON ������ ���� ���� 
	int RequestHandle = 0; // 100 ȸ������
	FCustomStruct event;

	//json �÷�������� ���� 
	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		FString tmpFlag = JsonObject->GetStringField("HandleFlag");
		RequestHandle = FCString::Atoi(*tmpFlag);

		switch (RequestHandle)
		{
		case 100: // ȸ������ �ڵ� 
			SignUpCheck(JsonObject);
			break;

		case 200: // �α��� �ڵ� 
			LoginCheck(JsonObject);
			break;

		case 300: // PostBox �ڵ鸵 �޼��� �˸��� -> ! <- �˸�
			httpHandleGetPostBoxSize(JsonObject, &event);
			EventHandlerData.Add(event);
			break;

		case 310: // getPostBox data �ڵ鸵  
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

		case 410: // Ƽ�ŷ, ��ü��ŷ ����Ʈ�� ó�� 
			SetTotalRank(JsonObject, &event);
			EventHandlerData.Add(event);
			break;

		case 500: // ���� �÷��� �α��ο� ��ϵ������� ����� ó��
			LoginCheckFlag = 4;
			break;

		case 510: // ���� �÷��� �г��� ��� ó�� �� ��Ī ȭ�� �̵� 
			GooglePlayLoginSuccess(JsonObject);
			LoginCheckFlag = 5;
			break;

		case 520: // ���� �÷��� �г��� ��� �г��� �ߺ� 
			LoginCheckFlag = 6; 
			break;

		case 530: // ���� �÷��� ���̵� ��ϵ� ���̵� 
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
	if (!MatChginConnCreateSocket("conn_MatchingServer", "115.71.232.4", 9887)) //���� ���� ����
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

	ConnectionSocket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("ClientSocket1"), false); // ������ ���� ����� 
	TSharedRef<FInternetAddr> RemoteAddress = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr(); // �ּ� ����

	RemoteAddress->SetIp(Addr.Value);
	RemoteAddress->SetPort(port);

	bool connected = ConnectionSocket->Connect(*RemoteAddress); //���� ���� 

	if (!connected)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connect fail.")));
		return false;
	}

	UWorld* World = GetWorld();
	World->GetTimerManager().SetTimer(TCPSocketListenerTimerHandle, this, &AAOWGameModeBase::TCPSocketListener, 0.5f, true); // socket ���ù�

	return true;
}

//���̳ʸ� ������ string���� ������ 
FString AAOWGameModeBase::StringFromBinaryArray(TArray<uint8> BinaryArray)
{
	//Create a string from a byte array!s
	const std::string cstr(reinterpret_cast<const char*>(BinaryArray.GetData()), BinaryArray.Num());
	return FString(cstr.c_str());

}

//�̹��� ���� ���� ���� 
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

// ���ڿ� ���̳ʸ� ����
void AAOWGameModeBase::BinaryArrayFromString(const FString& Message, TArray<uint8>& OutBinaryArray)
{
	FTCHARToUTF8 Convert(*Message);
	OutBinaryArray.Empty();
	OutBinaryArray.Append((UTF8CHAR*)Convert.Get(), Convert.Length());
}

//tcp recv
void AAOWGameModeBase::TCPSocketListener()
{
	if (!ConnectionSocket) //����conn?
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
		ConnectionSocket->Recv(ReceivedData.GetData(), ReceivedData.Num(), Read); //���ŷ �ƴ� 
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

// recv ��Ī �̺�Ʈ �ڵ鷯
void AAOWGameModeBase::recvMatchEventHandle(TSharedPtr<FJsonObject> JsonObject)
{

	FCustomStruct event;
	int eventFlag = JsonObject->GetIntegerField(TEXT("Event"));

	switch (eventFlag)
	{
	case 10: // ��Ī�������Ἲ��, ���� �⺻ ���� ���
		SendMessage(create_msg_event10());
		break;
	case 11: //2����Ī ��ȯ �̺�Ʈ 
		event.Event = 11;
		EventHandlerData.Add(event);
		break;
	case 12: //2����Ī ĵ�� �� ���� UI ����  
		LastMatchingEventHandling(JsonObject, &event);
		EventHandlerData.Add(event);
		break;
	case 13: // �޼��� ����? 
		event.Event = 13;
		EventHandlerData.Add(event);
		break;
	case 15: // 2�� ��Ī Ÿ�� �ƿ� 
		event.Event = 15;
		EventHandlerData.Add(event);
		break;

	case 20: // ��Ī ���� ���� ���� �ڵ鸵  
		setGameHandling(JsonObject, &event);
		EventHandlerData.Add(event);
		break;

	}

}

FString AAOWGameModeBase::create_msg_event10()
{
	FString tmpSessionID = SessionID.ToString(); // �̹� �����Ǿ��ִ� ������ 
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
	// �̷��� ����ȭ ����
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
	// ���� 
	if (LastMatchingType == 1)
		LastSuccessEventProcess(JsonObject, event);

	//ĵ��
	if (LastMatchingType == 2)
	{
		LastCancleEventProcess(JsonObject, event);
		time_t start = time(NULL);
		FTimerDelegate LastMatchingDelegate;
		LastMatchingDelegate.BindUFunction(this, FName("LastMatchingResetUI"), (float)start); // �����ִٰ� �ڵ����� 2����Ī ui â ���� ��Ű���̺�Ʈ 
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
	CountDownValue = 10; // ī��Ʈ �� �ʱ�ȭ 
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

	Chat_ConnectionSocket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("ChatClientSocket"), false); // ������ ���� ����� 
	TSharedRef<FInternetAddr> RemoteAddress = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr(); // �ּ� ����

	RemoteAddress->SetIp(Addr.Value);
	RemoteAddress->SetPort(port);

	bool connected = Chat_ConnectionSocket->Connect(*RemoteAddress); //���� ���� 

	if (!connected)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connect fail.")));
		return false;
	}

	UWorld* World = GetWorld();
	World->GetTimerManager().SetTimer(Chat_TCPSocketListenerTimerHandle, this, &AAOWGameModeBase::Chatting_TCPSocketListener, 0.5f, true); // socket ���ù�

	return true;

}

void AAOWGameModeBase::Chatting_TCPSocketListener()
{
	if (!Chat_ConnectionSocket) //����conn?
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
		Chat_ConnectionSocket->Recv(ReceivedData.GetData(), ReceivedData.Num(), Read); //���ŷ �ƴ� 

	}

	if (ReceivedData.Num() <= 0) {
		return;
	}

	//read
	FString recvmsg = StringFromBinaryArray(ReceivedData);
	TSharedPtr<FJsonObject> JsonObject; //Create a pointer to hold the json serialized data
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(recvmsg);
	FJsonSerializer::Deserialize(Reader, JsonObject);
	ChattingrecvEventHandle(JsonObject); // ���⼭ �������� -> ���� ����? ���� �¸��ϰ� �ٽ� ���������� �޼����� ���д�? �׷��Ű��� 
}


void AAOWGameModeBase::ChattingrecvEventHandle(TSharedPtr<FJsonObject> JsonObject)
{
	FCustomStruct event;
	int eventFlag = JsonObject->GetIntegerField(TEXT("Event"));

	switch (eventFlag) // 14 ��� ���� 16���� ��� ���� 
	{
	case 50: // ä�ü������Ἲ��, ���� �⺻ ���� ���
		Chatting_SendUserInfo(JsonObject);
		break;

	case 51: // ä�� ä�� ù����� ���� ��� ����ȭ 
		firstChannelUserList(JsonObject);
		break;

	case 60: // ä�κ� �ο��� ����ȭ 
		setChannelListSize(JsonObject);
		break;

	case 70: // ä�� �̵� ó��
		setChannelUserList(JsonObject, &event);
		EventHandlerData.Add(event);
		break;

	case 71: // ù ä�� ����� ä�� ui�� ������ ó��  
		break;

	case 80: // �ӽ� ä�� �޼��� 
		setChattingMsg(JsonObject, &event);
		EventHandlerData.Add(event);
		break;

	case 100: // ��� �޼��� ó�� 
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

//  chatting �޼��� send
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
	nowChannelNum = RoomNum; //���� ä�� �ο� ���� 
	UE_LOG(LogClass, Warning, TEXT("now chaanel-> %d"), nowChannelNum);
	FString tmpSessionID = SessionID.ToString(); // ���� ���̵� üũ�� �߿��ҵ� 
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

	for (int i = 0; i < arr.Num(); i++) //�迭�� �μ��� ����� �˼��ְ� 
	{
		int num = arr[i]->AsNumber();
		channellist.Emplace(num); //ä�� ���� ������ ���� 
	}
	ChannelListCheck = false;
}


TArray<int>& AAOWGameModeBase::getChannelList()
{
	ChannelListCheck = true; // �ʱⰪ���� �������� 
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
	event->ChannelNum = JsonObject->GetIntegerField("ChannelNum"); //ä�� ���� 
	nowChannelNum = JsonObject->GetIntegerField("ChannelNum"); // ����� ä�� ����
	TArray<TSharedPtr<FJsonValue>> arr = JsonObject->GetArrayField("ChannelUserList"); // ä�� ���� ��� 

	for (int i = 0; i < arr.Num(); i++) //������ �����ϴ°� ���� �Ϸ� 
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

// ��ŷ ���̶���Ʈ ü���� �Լ� 
void AAOWGameModeBase::RankingHighlightChange(int index) 
{
	RankingData[index].RankhighLigth = 1;
}