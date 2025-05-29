// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#pragma once

#include "CoreMinimal.h"
#include "../Item.h"
#include "Weapon.generated.h"

class USoundBase;
class UBoxComponent;

UCLASS()
class DELTA_API AWeapon : public AItem {
  GENERATED_BODY()

public:
  AWeapon();

  virtual void Tick(float DeltaTime) override;

  virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent,
                                    AActor*              OtherActor,
                                    UPrimitiveComponent* OtherComp,
                                    int32                OtherBodyIndex,
                                    bool                 bFromSweep,
                                    const FHitResult&    SweepResult) override;

  virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent,
                                  AActor*              OtherActor,
                                  UPrimitiveComponent* OtherComp,
                                  int32                OtherBodyIndex) override;

  UFUNCTION()
  virtual void OnWeaponBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent,
                                       AActor*              OtherActor,
                                       UPrimitiveComponent* OtherComp,
                                       int32                OtherBodyIndex,
                                       bool                 bFromSweep,
                                       const FHitResult&    SweepResult);

  virtual void Equip(USceneComponent* InParent, FName InSocketName);

  void AttackMeshToSocket(USceneComponent* const InParent, const FName InSocketName) const;

protected:
  virtual void BeginPlay() override;

  void DebugOverlap(AActor* OtherActor);

  UPROPERTY(EditAnywhere, Category = "Weapon Properties")
  TObjectPtr<USoundBase> EquipSound{nullptr};

  UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
  TObjectPtr<UBoxComponent> WeaponBox{nullptr};
};
