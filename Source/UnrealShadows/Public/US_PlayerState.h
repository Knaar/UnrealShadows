// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"
#include "US_PlayerState.generated.h"

#define Erreturn(Str) \
do { \
US_Utility::PrintError(TEXT(Str), this); \
return ; \
} while (0)


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnXPChanged,int32, NewXP);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterLevelUP, int32, NewLevel);

UCLASS()
class UNREALSHADOWS_API AUS_PlayerState : public APlayerState
{
	GENERATED_BODY()
protected:

	FTimerHandle XpTimerHandle;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	void AddXPDebag();
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, ReplicatedUsing="OnRep_Xp",Category="Experience")
	int32 Xp = 0;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,ReplicatedUsing="OnRep_CharacterLevel",Category="Experience")
	int32 CharacterLevel = 1;
	
	UFUNCTION()
	void OnRep_Xp(int32 OldValue);

	UFUNCTION()
	void OnRep_CharacterLevel(int32 OldValue);

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	public:
	UFUNCTION(BlueprintCallable, Category = "Experience")
	void AddXP(int32 Value);

protected:
	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnXPChanged OnXPChanged;
	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnCharacterLevelUP OnCharacterLevelUP;
};
