// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LiveLinkComponent.h"
#include "Common/UdpSocketReceiver.h"
#include "MyLiveLinkComponent.generated.h"

struct FIPv4Endpoint;
/**
 * 
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class LINK_LIVE_API UMyLiveLinkComponent : public ULiveLinkComponent
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	void getData(const FArrayReaderPtr& Reader, const FIPv4Endpoint& EndPt);

	UFUNCTION(BlueprintCallable, Category = "Recieved Data")
		TArray<FString> returnData();

	FSocket* socket;
	FIPv4Endpoint Endpoint;
	TArray<FString>Data;
	class FUdpSocketReceiver* Receiver = nullptr;

	void StopReceiver();
	void closeSocket();
	void conversion(FString str);
};
