// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BirdGameMode.generated.h"

UCLASS()
class DELTA_API ABirdGameMode : public AGameModeBase {
  GENERATED_BODY()

public:
  ABirdGameMode();

  virtual void InitGame(const FString& MapName,
                        const FString& Options, //
                        FString&       ErrorMessage) override;

  virtual void InitGameState() override;

  virtual void StartPlay() override;
};
