// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "US_GameState.h"
#include "US_PlayerController.h"
#include "US_PlayerState.h"
#include "US_Character.h"
#include "US_GameMode.generated.h"


UCLASS()
class UNREALSHADOWS_API AUS_GameMode : public AGameMode
{
	GENERATED_BODY()
public:
	AUS_GameMode();
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Defaults, meta = (AllowPrivateAccess))
	TSubclassOf<APawn> PlayerPawnClass;
};
