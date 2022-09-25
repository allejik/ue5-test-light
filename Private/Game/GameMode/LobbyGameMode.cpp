// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GameMode/LobbyGameMode.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (GameState) {
		const int32 NumberOfPlayers = GameState.Get()->PlayerArray.Num();

		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(
				1,
				60.0f,
				FColor::Yellow,
				FString::Printf(TEXT("Number of players: %d"), NumberOfPlayers)
			);

			const APlayerState* PlayerState = NewPlayer->GetPlayerState<APlayerState>();
			if (PlayerState) {
				const FString PlayerName = PlayerState->GetPlayerName();

				GEngine->AddOnScreenDebugMessage(
					-1,
					60.0f,
					FColor::Cyan,
					FString::Printf(TEXT("%s has joined the game"), *PlayerName)
				);
			}
		}
	}
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	const APlayerState* PlayerState = Exiting->GetPlayerState<APlayerState>();
	if (PlayerState) {
		const int32 NumberOfPlayers = GameState.Get()->PlayerArray.Num();

		GEngine->AddOnScreenDebugMessage(
			1,
			60.0f,
			FColor::Yellow,
			FString::Printf(TEXT("Number of players: %d"), NumberOfPlayers - 1)
		);

		const FString PlayerName = PlayerState->GetPlayerName();

		GEngine->AddOnScreenDebugMessage(
			-1,
			60.0f,
			FColor::Cyan,
			FString::Printf(TEXT("%s has exited the game"), *PlayerName)
		);
	}
}
