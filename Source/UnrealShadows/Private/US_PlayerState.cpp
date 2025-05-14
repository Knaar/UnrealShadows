// Fill out your copyright notice in the Description page of Project Settings.


#include "US_PlayerState.h"
#include "US_Character.h"
#include "US_CharacterStats.h"
#include "Net/UnrealNetwork.h"
#include "US_Utility.h"

void AUS_PlayerState::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority())
	{
		GetWorld()->GetTimerManager().SetTimer(
			XpTimerHandle,
			this,
			&AUS_PlayerState::AddXPDebag,
			30.0f,
			true
		);
	}
}

void AUS_PlayerState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	GEngine->AddOnScreenDebugMessage(0, DeltaSeconds, FColor::Yellow, FString::Printf(TEXT("ScoreIs: %d"), Xp));
}

void AUS_PlayerState::AddXPDebag()
{
	//AddXP(25);
}

void AUS_PlayerState::OnRep_Xp(int32 OldValue)
{
	OnXPChanged.Broadcast(Xp);
}

void AUS_PlayerState::OnRep_CharacterLevel(int32 OldValue)
{
	OnCharacterLevelUP.Broadcast(CharacterLevel);
}

void AUS_PlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(AUS_PlayerState, Xp, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AUS_PlayerState, CharacterLevel, COND_OwnerOnly);
}

void AUS_PlayerState::AddXP(int32 Value)
{
	Xp += Value;
	OnXPChanged.Broadcast(Value);
	GEngine->AddOnScreenDebugMessage(1, 5, FColor::Red, FString::Printf(TEXT("Xp: %d"), Value));

	APawn* Pawn;
	auto PC = Cast<APlayerController>(GetOwner());
	if (PC)
	{
		Pawn = PC->GetPawn();
	}
	else
	{
		Pawn = GetPawn();
	}
	
	if (!Pawn) Erreturn("US_PlayerState can't find Pawn");
	auto Character= Cast<AUS_Character>(Pawn);
	if (!Character) Erreturn("US_PlayerState Cast Pawn Failed");
	auto Stats = Character->GetCharacterStats();
	if (Stats==nullptr) Erreturn("US_PlayerState CharacterStats Failed");
	
	if (Stats->NextLevelXp<Xp)
	{
		GEngine->AddOnScreenDebugMessage(1, 5, FColor::Red, FString::Printf(TEXT("Level Up")));
		CharacterLevel++;
		Character->UpdateCharacterStats(CharacterLevel);
		OnCharacterLevelUP.Broadcast(CharacterLevel);
	}
}
