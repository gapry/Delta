// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Sword.generated.h"

UCLASS()
class DELTA_API ASword : public AWeapon {
  GENERATED_BODY()

public:
  ASword();

  virtual void Equip(USceneComponent* InParent, FName InSocketName) override;

  virtual void OnWeaponBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent,
                                       AActor*              OtherActor,
                                       UPrimitiveComponent* OtherComp,
                                       int32                OtherBodyIndex,
                                       bool                 bFromSweep,
                                       const FHitResult&    SweepResult) override;
};
