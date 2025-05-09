#pragma once

#include "CoreMinimal.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

namespace US_Utility
{
	inline void PrintError(const FString& Message, UObject* WorldContext = nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("❌ %s"), *Message);

		if (GEngine)
		{
			UWorld* World = WorldContext ? WorldContext->GetWorld() : nullptr;
			APlayerController* PC = World ? World->GetFirstPlayerController() : nullptr;

			if (PC)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("❌ %s"), *Message));
			}
		}
	}
}