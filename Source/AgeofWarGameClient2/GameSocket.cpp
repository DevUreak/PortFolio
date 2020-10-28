// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSocket.h"
#include "Engine/Engine.h"
#include "Serialization/JsonReader.h"
#include "FSocketMessageHeader.h"
#include "Serialization/JsonSerializer.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/BlueprintGeneratedClass.h"
#include "string"

GameSocket::GameSocket(UAOWGameInstance* tgameInstance, class AutoGameTime* tautotimer)
{
	gameInstance = tgameInstance;
	autotimer = tautotimer;
	GameConnCreateSocket("conn_gameServer", "192.168.43.129", 7777); // 게임 서버 연결 
}

uint32 GameSocket::Run()
{
	ThreadRuning = true;
	while (ThreadRuning)
	{
		if (!ConnGameSocket) //소켓conn check
		{
			UE_LOG(LogClass, Warning, TEXT("Conn not!!!"));
			//continue;
		}

		//Binary Array!
		TArray<uint8> ReceivedData;
		uint32 Size;

		while (ConnGameSocket->HasPendingData(Size))
		{
			int32 Read = 0;
			ReceivedData.Init(FMath::Min(Size, 65507u), Size);
			ConnGameSocket->Recv(ReceivedData.GetData(), ReceivedData.Num(), Read); //블로킹 아님 
		}

		if (ReceivedData.Num() <= 0)
		{
			//continue;
		}
		else
		{
			FString recvmsg = StringFromBinaryArray(ReceivedData);
			TSharedPtr<FJsonObject> JsonObject; //Create a pointer to hold the json serialized data
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(recvmsg);
			FJsonSerializer::Deserialize(Reader, JsonObject);
			recvMatchEventHandle(JsonObject);
		}
	}
	return 0;
}


void GameSocket::Exit()
{
	ThreadRuning = false;
	// Here's where we can do any cleanup we want to 
}


void GameSocket::Stop()
{
	ThreadRuning = false;
	// Force our thread to stop early
}


void GameSocket::recvMatchEventHandle(TSharedPtr<FJsonObject> JsonObject)
{
	FCustomStructGPacket packet;
	int eventFlag = JsonObject->GetIntegerField(TEXT("Event"));

	switch (eventFlag)
	{
	case 1: // 게임 서버 연동, 데이터 초기화, 방생성or방입장 S
		SendMessage(getGameBaseData());
		
		break;
	case 20: // 20부터 게임 이벤트 
		UE_LOG(LogClass, Warning, TEXT("GAME EVENT 20"));
		autotimer->StartAutoTime(); // 게임 프레임 타임 시작 
		ActiveGame(JsonObject,&packet);
		gameInstance->TEventGameData.Add(packet);
		break;

	case 21: // 상대방 살아있다는 메세지 
		UE_LOG(LogClass, Warning, TEXT("GAME EVENT 21"));
		OpperReadyConfirm();
		break;

	case 22: // 병사 생산이벤트 처리 -> 생산 시스템으로 넘겨줘야함 
		SetProductMsg(JsonObject);
		UE_LOG(LogClass, Warning, TEXT("GAME EVENT 22"));
		break;

	}

}


// send msg
bool GameSocket::SendMessage(const FString& Message)
{
	if (ConnGameSocket)
	{
		TArray<uint8> Payload;
		BinaryArrayFromString(Message, Payload);
		FSocketMessageHeader::WrapAndSendPayload(Payload, ConnGameSocket);
		FString recvmsg = StringFromBinaryArray(Payload);
		return true;
	}
	return false;
}

// 문자열 바이너리 변형
void GameSocket::BinaryArrayFromString(const FString& Message, TArray<uint8>& OutBinaryArray)
{
	FTCHARToUTF8 Convert(*Message);
	OutBinaryArray.Empty();
	OutBinaryArray.Append((UTF8CHAR*)Convert.Get(), Convert.Length());
}

//바이너리 데이터 string으로 포맷팅 
FString GameSocket::StringFromBinaryArray(TArray<uint8> BinaryArray)
{
	//Create a string from a byte array!s
	const std::string cstr(reinterpret_cast<const char*>(BinaryArray.GetData()), BinaryArray.Num());
	return FString(cstr.c_str());

}

//IP Parser
bool GameSocket::Parse(const FString& MaskString, FIPv4Address& OutMask)
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

// 게임 서버 연결 
bool GameSocket::GameConnCreateSocket(const FString& YourChosenSocketName, const FString& TheIP, const int32 ThePort)
{
	FIPv4Address Addr;
	Parse(TheIP, Addr);

	ConnGameSocket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("Client"), false); // 연결할 소켓 만들기 
	TSharedRef<FInternetAddr> RemoteAddress = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr(); // 주소 생성

	RemoteAddress->SetIp(Addr.Value);
	RemoteAddress->SetPort(ThePort);

	bool connected = ConnGameSocket->Connect(*RemoteAddress); //소켓 연결 

	if (!connected)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connect Game Fail.")));
		return false;
	}

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connect Game Success.")));

	return true;
}

//게임 준비좀하게 정보좀 줘봐 
FString GameSocket::getGameBaseData()
{
	//나중에 여기다가 함수하나 추가해서 재접속관련해서 재접속인지 게임방 생성인지 구분하는 로직을 추가하자고 
	FString OutputString;
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	JsonObject->SetNumberField("Event", 10);
	JsonObject->SetStringField("game_Sessionid", *gameInstance->fsGameOption.game_sessionid); // 게임 모드에서 변수 읽은 뒤에 게임 인스턴스 소스에다가 넣자 
	JsonObject->SetStringField("game_Macid", *gameInstance->fsGameOption.game_macid);
	JsonObject->SetStringField("game_Ggameindex",*gameInstance->fsGameOption.game_gameindex);
	JsonObject->SetStringField("game_UserNickName",*gameInstance->fsGameOption.game_usernickname);
	JsonObject->SetNumberField("UserType", gameInstance->fsGameOption.userType);

	TSharedRef< TJsonWriter<> > Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	/*UBlueprintGeneratedClass* LoadBP = LoadObject<UBlueprintGeneratedClass>(NULL, TEXT("/Game/GameLevel/BluePrint/GameInstanceBP.GameInstanceBP"), NULL, LOAD_None, NULL);
	gameInstance = Cast<UAOWGameInstance>(LoadBP);*/ // -> 필드에있는 블루프린트 객체 얻기 

	return OutputString;
}

//게임 활성화좀 
void GameSocket::ActiveGame(TSharedPtr<FJsonObject> JsonObject, FCustomStructGPacket* packet)
{
	int Event = JsonObject->GetNumberField("Event");
	packet->msgType = Event;
}

//상대방 살아있데 
void GameSocket::OpperReadyConfirm()
{
	gameInstance->opperReadState = true;
}

// 진짜 게임 준비 완료 전송 
void GameSocket::S_GameReadyComplete() 
{
	FString OutputString;
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	JsonObject->SetNumberField("Event", 11);
	JsonObject->SetStringField("game_Ggameindex", *gameInstance->fsGameOption.game_gameindex);
	JsonObject->SetNumberField("UserType", gameInstance->fsGameOption.userType);
	TSharedRef< TJsonWriter<> > Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
	SendMessage(OutputString);
}

void GameSocket::S_ProductMonsterBear(int spawnPosition, int SpawnTime)
{
	FString OutputString;
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);

	JsonObject->SetNumberField("Event", 20);
	JsonObject->SetStringField("game_Ggameindex", *gameInstance->fsGameOption.game_gameindex);
	JsonObject->SetNumberField("UserType", gameInstance->fsGameOption.userType);
	JsonObject->SetNumberField("SpawnPosition", spawnPosition);
	JsonObject->SetNumberField("SoliderType", 1);
	JsonObject->SetNumberField("SpawnTime", SpawnTime);

	TSharedRef< TJsonWriter<> > Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
	SendMessage(OutputString);

}

// 원거리 머독 생산 전송  메세지 
void GameSocket::S_ProductMonsterMurdock(int spawnPosition, int SpawnTime)
{
	FString OutputString;
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);

	JsonObject->SetNumberField("Event", 20);
	JsonObject->SetStringField("game_Ggameindex", *gameInstance->fsGameOption.game_gameindex);
	JsonObject->SetNumberField("UserType", gameInstance->fsGameOption.userType);
	JsonObject->SetNumberField("SpawnPosition", spawnPosition);
	JsonObject->SetNumberField("SoliderType", 2);
	JsonObject->SetNumberField("SpawnTime", SpawnTime);

	TSharedRef< TJsonWriter<> > Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
	SendMessage(OutputString);


}


// 생산 시스템으로 이동 
void GameSocket::SetProductMsg(TSharedPtr<FJsonObject> JsonObject)
{
	int requestUserType = JsonObject->GetIntegerField("UserType"); // 1==왼쪽 2==오른쪽 이벤트 
	int soliderType = JsonObject->GetIntegerField("SoliderType"); 
	int spawnPosition = JsonObject->GetIntegerField("SpawnPosition");
	int completeTime = JsonObject->GetIntegerField("CompleteTime"); 

	FCustomStructProduct fcsp;
	fcsp.ProductEventType = 23;
	fcsp.RequestUserType = requestUserType;
	fcsp.SoliderType = soliderType;
	fcsp.SpawnPosition = spawnPosition;
	fcsp.SpawnCompleteTime = completeTime;
	fcsp.SpawnPosible = false;
	gameInstance->Product_GPacket.Emplace(fcsp);
}