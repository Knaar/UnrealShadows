// Fill out your copyright notice in the Description page of Project Settings.


#include "US_GameMode.h"

#include "UObject/ConstructorHelpers.h"

AUS_GameMode::AUS_GameMode()
{
	GameStateClass = AUS_GameState::StaticClass();
	PlayerStateClass = AUS_PlayerState::StaticClass();
	PlayerControllerClass = AUS_PlayerController::StaticClass();

	static ConstructorHelpers::FClassFinder<APawn>PlayerPawnBPClass(TEXT("/Game/Blueprints/BP_Character"));
	
	if (PlayerPawnClass != nullptr)
	{
		DefaultPawnClass = PlayerPawnClass;
	}
	else if(PlayerPawnBPClass.Class != nullptr)
	{
			DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	else
	{
		UE_LOG(LogTemp,Error,TEXT("Failed to find a BP_Character"))
	}
	
};
