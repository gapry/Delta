// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#pragma once

#include "CoreMinimal.h"
#include "Field/FieldSystemActor.h"
#include "Field/FieldSystemObjects.h"
#include "../Item/Item.h"
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

  virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, //
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

  virtual void Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator);

  void AttackMeshToSocket(USceneComponent* const InParent, const FName InSocketName) const;

  UBoxComponent* GetWeaponBox() const;

  TArray<TObjectPtr<AActor>> IgnoreActors;

protected:
  virtual void BeginPlay() override;

  void DebugOverlap(AActor* OtherActor);

  void ApplyRadialFalloffField(float                 Magnitude,
                               float                 MinRange,
                               float                 MaxRange,
                               float                 Default,
                               float                 Radius,
                               const FVector&        Position,
                               EFieldFalloffType     FalloffType,
                               bool                  bEnabled,
                               EFieldPhysicsType     PhysicsType,
                               UFieldSystemMetaData* MetaData);

  void ApplyRadialVectorField(float Magnitude, const FVector& Position, bool bEnabled, EFieldPhysicsType PhysicsType, UFieldSystemMetaData* MetaData);

  virtual void CreateAttackFields(const FVector& ImpactPoint);

  UPROPERTY(EditAnywhere, Category = "Weapon Properties")
  TObjectPtr<USoundBase> EquipSound{nullptr};

  UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
  TObjectPtr<UBoxComponent> WeaponBox{nullptr};

  UPROPERTY(EditAnywhere, Category = "Weapon Properties")
  TObjectPtr<USceneComponent> BoxTraceStart{nullptr};

  UPROPERTY(EditAnywhere, Category = "Weapon Properties")
  TObjectPtr<USceneComponent> BoxTraceEnd{nullptr};

  UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"))
  TObjectPtr<UFieldSystemComponent> FieldSystemComponent{nullptr};

  UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"))
  TObjectPtr<URadialFalloff> RadialFalloffComponent{nullptr};

  UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"))
  TObjectPtr<URadialVector> RadialVectorComponent{nullptr};

  UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"))
  TObjectPtr<UFieldSystemMetaDataFilter> FieldSystemMetaDataFilterComponent{nullptr};

  UPROPERTY(EditAnywhere, Category = "Weapon Properties")
  float Damage{20.0f};
};
