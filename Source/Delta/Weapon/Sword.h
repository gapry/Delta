// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Sword.generated.h"

#define DELTA_SWORD_ENABLE_DEBUG_HIT               0
#define DELTA_SWORD_ENABLE_BLUEPRINT_ATTACK_FIELDS 0

UCLASS()
class DELTA_API ASword : public AWeapon {
  GENERATED_BODY()

public:
  ASword();

  virtual void Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator) override;

  virtual void OnWeaponBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent,
                                       AActor*              OtherActor,
                                       UPrimitiveComponent* OtherComp,
                                       int32                OtherBodyIndex,
                                       bool                 bFromSweep,
                                       const FHitResult&    SweepResult) override;

  void CreateAttackFields(const FVector& FieldLocation) override;

protected:
  UFUNCTION(BlueprintImplementableEvent)
  void CreateBP_AttackFields(const FVector& FieldLocation);

  void CreateNative_AttackFields(const FVector& ImpactPoint);
};
