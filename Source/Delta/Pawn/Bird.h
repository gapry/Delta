// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Bird.generated.h"

class UInputComponent;
class USkeletalMeshComponent;

UCLASS()
class DELTA_API ABird : public APawn {
  GENERATED_BODY()

public:
  ABird();

  virtual void Tick(float DeltaTime) override;

  virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

  void InitializeSkeletalMeshComponent(const TCHAR* const SkeletalMeshPath);

protected:
  virtual void BeginPlay() override;

private:
  UPROPERTY(VisibleAnywhere,
            BlueprintReadOnly,
            Category = "Bird",
            meta     = (AllowPrivateAccess = "true"))
  TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent{nullptr};
};
