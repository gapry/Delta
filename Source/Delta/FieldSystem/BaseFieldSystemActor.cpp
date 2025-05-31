// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#include "BaseFieldSystemActor.h"
#include "Field/FieldSystemComponent.h"
#include "Field/FieldSystemTypes.h"
#include "Kismet/KismetSystemLibrary.h"

ABaseFieldSystemActor::ABaseFieldSystemActor() {
  PrimaryActorTick.bCanEverTick = true;

  RadialFalloffComponent = CreateDefaultSubobject<URadialFalloff>(TEXT("RadialFalloffComponent"));
  check(RadialFalloffComponent);

  RadialVectorComponent = CreateDefaultSubobject<URadialVector>(TEXT("RadialVectorComponent"));
  check(RadialVectorComponent);

  FieldSystemMetaDataFilterComponent = CreateDefaultSubobject<UFieldSystemMetaDataFilter>(TEXT("FieldSystemMetaDataFilterComponent"));
  check(FieldSystemMetaDataFilterComponent);

  FieldSystemMetaDataFilterComponent->FilterType   = EFieldFilterType::Field_Filter_Dynamic;
  FieldSystemMetaDataFilterComponent->ObjectType   = EFieldObjectType::Field_Object_Destruction;
  FieldSystemMetaDataFilterComponent->PositionType = EFieldPositionType::Field_Position_CenterOfMass;
}

void ABaseFieldSystemActor::BeginPlay() {
  Super::BeginPlay();

  if (RadialFalloffComponent == nullptr || FieldSystemComponent == nullptr) {
    return;
  }

  static const FVector ActorLocation = GetActorLocation();

  ApplyRadialFalloffField(1000000.0f,
                          0.8f,
                          1.0f,
                          0.0f,
                          500.0f,
                          ActorLocation,
                          EFieldFalloffType::Field_FallOff_None,
                          true,
                          EFieldPhysicsType::Field_ExternalClusterStrain,
                          nullptr);

  ApplyRadialVectorField(5000000.0f, ActorLocation, true, EFieldPhysicsType::Field_LinearForce, FieldSystemMetaDataFilterComponent);
}

void ABaseFieldSystemActor::ApplyRadialFalloffField(float                 Magnitude,
                                                    float                 MinRange,
                                                    float                 MaxRange,
                                                    float                 Default,
                                                    float                 Radius,
                                                    const FVector&        Position,
                                                    EFieldFalloffType     FalloffType,
                                                    bool                  bEnabled,
                                                    EFieldPhysicsType     PhysicsType,
                                                    UFieldSystemMetaData* MetaData) {
  if (RadialFalloffComponent == nullptr || FieldSystemComponent == nullptr) {
    return;
  }

  auto* const FieldNode = RadialFalloffComponent->SetRadialFalloff(Magnitude, //
                                                                   MinRange,
                                                                   MaxRange,
                                                                   Default,
                                                                   Radius,
                                                                   Position,
                                                                   FalloffType);

  FieldSystemComponent->ApplyPhysicsField(bEnabled, PhysicsType, MetaData, FieldNode);
}

void ABaseFieldSystemActor::ApplyRadialVectorField(float                 Magnitude,
                                                   const FVector&        Position,
                                                   bool                  bEnabled,
                                                   EFieldPhysicsType     PhysicsType,
                                                   UFieldSystemMetaData* MetaData) {
  if (RadialVectorComponent == nullptr || FieldSystemComponent == nullptr) {
    return;
  }

  auto* const FieldNode = RadialVectorComponent->SetRadialVector(Magnitude, Position);
  FieldSystemComponent->ApplyPhysicsField(bEnabled, PhysicsType, MetaData, FieldNode);
}