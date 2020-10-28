// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Networking.h"
#include "NetworkMessage.h"
#include "FSocketMessageHeader.h"


/**
 * 
 */

class AGEOFWARGAMECLIENT2_API FSocketMessageHeader
{
public:
	/** Error checking */
	uint32 Magic = 0;

	/** Payload Size */
	uint32 PayloadSize = 0;

	static uint32 DefaultMagic;
public:
	FSocketMessageHeader(const TArray<uint8>& Payload)
	{
		PayloadSize = Payload.Num();  // What if PayloadSize is 0
		Magic = FSocketMessageHeader::DefaultMagic;
	}

	/** Add header to payload and send it out */
	static bool WrapAndSendPayload(const TArray<uint8>& Payload, FSocket* Socket);
	/** Receive packages and strip header */
	static bool ReceivePayload(FArrayReader& OutPayload, FSocket* Socket);
};
