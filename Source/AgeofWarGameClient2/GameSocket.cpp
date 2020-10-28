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
	GameConnCreateSocket("conn_gameServer", "192.168.43.129", 7777); // ���� ���� ���� 
}

uint32 GameSocket::Run()
{
	ThreadRuning = true;
	while (ThreadRuning)
	{
		if (!ConnGameSocket) //����conn check
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
			ConnGameSocket->Recv(ReceivedData.GetData(), ReceivedData.Num(), Read); //���ŷ �ƴ� 
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
	case 1: // ���� ���� ����, ������ �ʱ�ȭ, �����or������ S
		SendMessage(getGameBaseData());
		
		break;
	case 20: // 20���� ���� �̺�Ʈ 
		UE_LOG(LogClass, Warning, TEXT("GAME EVENT 20"));
		autotimer->StartAutoTime(); // ���� ������ Ÿ�� ���� 
		ActiveGame(JsonObject,&packet);
		gameInstance->TEventGameData.Add(packet);
		break;

	case 21: // ���� ����ִٴ� �޼��� 
		UE_LOG(LogClass, Warning, TEXT("GAME EVENT 21"));
		OpperReadyConfirm();
		break;

	case 22: // ���� �����̺�Ʈ ó�� -> ���� �ý������� �Ѱ������ 
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

// ���ڿ� ���̳ʸ� ����
void GameSocket::BinaryArrayFromString(const FString& Message, TArray<uint8>& OutBinaryArray)
{
	FTCHARToUTF8 Convert(*Message);
	OutBinaryArray.Empty();
	OutBinaryArray.Append((UTF8CHAR*)Convert.Get(), Convert.Length());
}

//���̳ʸ� ������ string���� ������ 
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

// ���� ���� ���� 
bool GameSocket::GameConnCreateSocket(const FString& YourChosenSocketName, const FString& TheIP, const int32 ThePort)
{
	FIPv4Address Addr;
	Parse(TheIP, Addr);

	ConnGameSocket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("Client"), false); // ������ ���� ����� 
	TSharedRef<FInternetAddr> RemoteAddress = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr(); // �ּ� ����

	RemoteAddress->SetIp(Addr.Value);
	RemoteAddress->SetPort(ThePort);

	bool connected = ConnGameSocket->Connect(*RemoteAddress); //���� ���� 

	if (!connected)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connect Game Fail.")));
		return false;
	}

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connect Game Success.")));

	return true;
}

//���� �غ����ϰ� ������ ��� 
FString GameSocket::getGameBaseData()
{
	//���߿� ����ٰ� �Լ��ϳ� �߰��ؼ� �����Ӱ����ؼ� ���������� ���ӹ� �������� �����ϴ� ������ �߰����ڰ� 
	FString OutputString;
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	JsonObject->SetNumberField("Event", 10);
	JsonObject->SetStringField("game_Sessionid", *gameInstance->fsGameOption.game_sessionid); // ���� ��忡�� ���� ���� �ڿ� ���� �ν��Ͻ� �ҽ����ٰ� ���� 
	JsonObject->SetStringField("game_Macid", *gameInstance->fsGameOption.game_macid);
	JsonObject->SetStringField("game_Ggameindex",*gameInstance->fsGameOption.game_gameindex);
	JsonObject->SetStringField("game_UserNickName",*gameInstance->fsGameOption.game_usernickname);
	JsonObject->SetNumberField("UserType", gameInstance->fsGameOption.userType);

	TSharedRef< TJsonWriter<> > Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	/*UBlueprintGeneratedClass* LoadBP = LoadObject<UBlueprintGeneratedClass>(NULL, TEXT("/Game/GameLevel/BluePrint/GameInstanceBP.GameInstanceBP"), NULL, LOAD_None, NULL);
	gameInstance = Cast<UAOWGameInstance>(LoadBP);*/ // -> �ʵ忡�ִ� �������Ʈ ��ü ��� 

	return OutputString;
}

//���� Ȱ��ȭ�� 
void GameSocket::ActiveGame(TSharedPtr<FJsonObject> JsonObject, FCustomStructGPacket* packet)
{
	int Event = JsonObject->GetNumberField("Event");
	packet->msgType = Event;
}

//���� ����ֵ� 
void GameSocket::OpperReadyConfirm()
{
	gameInstance->opperReadState = true;
}

// ��¥ ���� �غ� �Ϸ� ���� 
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

// ���Ÿ� �ӵ� ���� ����  �޼��� 
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


// ���� �ý������� �̵� 
void GameSocket::SetProductMsg(TSharedPtr<FJsonObject> JsonObject)
{
	int requestUserType = JsonObject->GetIntegerField("UserType"); // 1==���� 2==������ �̺�Ʈ 
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