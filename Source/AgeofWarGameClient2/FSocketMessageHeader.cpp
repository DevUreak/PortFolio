// Fill out your copyright notice in the Description page of Project Settings.


#include "FSocketMessageHeader.h"
#include "Networking.h"
#include "Engine/Engine.h"
#include "Math/UnrealMathUtility.h"

//�޼��� ���۽� ���ε� 

bool FSocketMessageHeader::WrapAndSendPayload(const TArray<uint8>& Payload, FSocket* Socket)
{
	FSocketMessageHeader Header(Payload);

	FBufferArchive Ar;
	Ar << Header.Magic;
	Ar << Header.PayloadSize;
	Ar.Append(Payload);

	int32 AmountSent;
	Socket->Send(Ar.GetData(), Ar.Num(), AmountSent); //���⼭ ���� ����
	if (AmountSent != Ar.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to send."));
		return false;
	}
	UE_LOG(LogTemp, Error, TEXT("to send"));
	Ar.Close();
	return true;
}