// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#include "BirdGameMode.h"
#include "GameFramework/PlayerController.h"
#include "../Player/Bird/Bird.h"
#include "../Common/LogUtil.h"

ABirdGameMode::ABirdGameMode() {
  PlayerStateClass      = nullptr;
  GameStateClass        = nullptr;
  SpectatorClass        = nullptr;
  HUDClass              = nullptr;
  DefaultPawnClass      = ABird::StaticClass();
  PlayerControllerClass = APlayerController::StaticClass();
}

void ABirdGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) {
  Super::InitGame(MapName, Options, ErrorMessage);
}

void ABirdGameMode::InitGameState() {
  Super::InitGameState();
}

void ABirdGameMode::StartPlay() {
  Super::StartPlay();
}