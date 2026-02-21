// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
// ----------------------------------------------------------------------------
// File: CNetStatics.h
// Purpose: Blueprint-accessible static helpers to interact with the Online
//          Subsystem (sessions/identity), build session settings, and read
//          command line configuration for coordinator URLs, ports, etc.
// ----------------------------------------------------------------------------
#include "CNetStatics.generated.h"

/**
 * Static utility library for networking/session helpers. Centralizes access to
 * OnlineSubsystem pointers, common session keys (names/ports/search IDs), and
 * command line parsing for server/client configuration. Usable from C++ and BP.
 */
UCLASS()
class UCNetStatics : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()
public: 
    // Construct FOnlineSessionSettings with project-standard flags/keys
    static FOnlineSessionSettings GenerateOnlineSesisonSettings(const FName& SessionName, const FString& SessionSearchId, int Port);

    static IOnlineSessionPtr GetSessionPtr();
    static IOnlineIdentityPtr GetIdentityPtr();

    // Returns configured players per team (e.g., from INI/defines)
    static uint8 GetPlayerCountPerTeam();

    // True if the current context is running as the session host/server
    static bool IsSessionServer(const UObject* WorldContextObject);

    static FString GetSessionNameStr();
    static FName GetSessionNameKey();

    static FString GetSesisonSearchIdStr();
    static FName GetSessionSearchIdKey();

    // Returns the port the session advertises/uses
    static int GetSessionPort();
    static FName GetPortKey();

    static FName GetCoordinatorURLKey();
    static FString GetCoordinatorURL();
    static FString GetDefaultCoordinatorURL();

    // Pulls values from command line: -ParamName=Value
    static FString GetCommandlineArgAsString(const FName& ParamName);
    static int GetCommandlineArgAsInt(const FName& ParamName);

    static FString GetTestingURL();
    static FName GetTestingURLKey();

    // Utility to swap port number in a URL string
    static void ReplacePort(FString& OutURLStr, int NewPort);
  };
