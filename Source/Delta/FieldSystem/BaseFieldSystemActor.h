// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#pragma once

#include "CoreMinimal.h"
#include "Field/FieldSystemActor.h"
#include "Field/FieldSystemObjects.h"
#include "BaseFieldSystemActor.generated.h"

UCLASS()
class DELTA_API ABaseFieldSystemActor : public AFieldSystemActor {
  GENERATED_BODY()

public:
  ABaseFieldSystemActor();

protected:
  virtual void BeginPlay() override;

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

private:
  UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"))
  TObjectPtr<URadialFalloff> RadialFalloffComponent{nullptr};

  UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"))
  TObjectPtr<URadialVector> RadialVectorComponent{nullptr};

  UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"))
  TObjectPtr<UFieldSystemMetaDataFilter> FieldSystemMetaDataFilterComponent{nullptr};
};
