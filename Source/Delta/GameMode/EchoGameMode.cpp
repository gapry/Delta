// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#include "EchoGameMode.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/GameStateBase.h"

#include "../Character/EchoCharacter.h"

AEchoGameMode::AEchoGameMode() {
  PlayerStateClass      = APlayerState::StaticClass();
  GameStateClass        = AGameStateBase::StaticClass();
  SpectatorClass        = nullptr;
  HUDClass              = nullptr;
  DefaultPawnClass      = AEchoCharacter::StaticClass();
  PlayerControllerClass = APlayerController::StaticClass();
}
