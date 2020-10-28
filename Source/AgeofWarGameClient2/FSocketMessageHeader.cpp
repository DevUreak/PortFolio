// Fill out your copyright notice in the Description page of Project Settings.


#include "FSocketMessageHeader.h"
#include "Networking.h"
#include "Engine/Engine.h"
#include "Math/UnrealMathUtility.h"

//메세지 전송시 바인딩 

bool FSocketMessageHeader::WrapAndSendPayload(const TArray<uint8>& Payload, FSocket* Socket)
{
	FSocketMessageHeader Header(Payload);

	FBufferArchive Ar;
	Ar << Header.Magic;
	Ar << Header.PayloadSize;
	Ar.Append(Payload);

	int32 AmountSent;
	Socket->Send(Ar.GetData(), Ar.Num(), AmountSent); //여기서 최종 전송
	if (AmountSent != Ar.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to send."));
		return false;
	}
	UE_LOG(LogTemp, Error, TEXT("to send"));
	Ar.Close();
	return true;
}