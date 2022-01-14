// Fill out your copyright notice in the Description page of Project Settings.


#include "MyLiveLinkComponent.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "Common/UdpSocketBuilder.h"
#include "Common/UdpSocketReceiver.h"
#include "Interfaces/IPv4/IPv4Endpoint.h"
#include "Networking/Public/Interfaces/IPv4/IPv4Address.h"
#include "Networking/Public/Interfaces/IPv4/IPv4SubnetMask.h"
#include "Runtime/Core/Public/Templates/SharedPointer.h"


void UMyLiveLinkComponent::BeginPlay()
{
    Super::BeginPlay();
    FString address = TEXT("127.0.0.1");
    int32 port = 7777;
    FIPv4Address ip;
    FIPv4Address::Parse(address, ip);
    Endpoint=FIPv4Endpoint(ip, port);

    int32 BufferSize = 1024;

    socket = FUdpSocketBuilder(TEXT("Receiver socket"))
    .AsNonBlocking()
	.AsReusable()
	.BoundToEndpoint(Endpoint)
	.WithReceiveBufferSize(BufferSize);

    if (!socket)
    {
        UE_LOG(LogTemp, Error, TEXT("Build Socket failed."));
        closeSocket();
        return;
    }

    FTimespan ThreadWaitTime = FTimespan::FromMilliseconds(100);
    Receiver = new FUdpSocketReceiver(socket, ThreadWaitTime, TEXT("UDP RECEIVER"));
    if (!Receiver)
    {
        UE_LOG(LogTemp, Error, TEXT("Receiver failed."));
        StopReceiver();
        return;
    }
    Receiver->OnDataReceived().BindUObject(this, &UMyLiveLinkComponent::getData);
    Receiver->Start();
}
void UMyLiveLinkComponent::getData(const FArrayReaderPtr& Reader, const FIPv4Endpoint& EndPt)
{
    FString DataStr = FString::FromHexBlob(Reader->GetData(), Reader->Num());
	UE_LOG(LogTemp, Warning, TEXT("Receiving:%s"),*DataStr);
    conversion(DataStr);
}

TArray<FString> UMyLiveLinkComponent::returnData()
{
    return Data;
}

void UMyLiveLinkComponent::StopReceiver()
{
    Receiver->Stop();
        delete Receiver;
    Receiver = nullptr;
}

void UMyLiveLinkComponent::closeSocket()
{
    socket->Close();
    delete socket;
    socket = nullptr;
}

 void UMyLiveLinkComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
     Super::EndPlay(EndPlayReason);
     StopReceiver();
     closeSocket();
}

void UMyLiveLinkComponent::conversion(FString str)
{
    FString temp;
	for(int i=0;i<str.Len();i++)
	{
		if(i%2!=0)
		{
            temp.AppendChar(str[i]);
		}
	}
    Data.Push(temp);
    temp.Reset();
}