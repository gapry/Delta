// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EchoCharacter.generated.h"

class USkeletalMeshComponent;

UCLASS()
class DELTA_API AEchoCharacter : public ACharacter {
  GENERATED_BODY()

public:
  AEchoCharacter();

  virtual void Tick(float DeltaTime) override;

  virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
  virtual void BeginPlay() override;
  virtual void PostInitializeComponents() override;

  void PostInitializeSkeletalMeshComponent();

private:
  TWeakObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent{GetMesh()};
};