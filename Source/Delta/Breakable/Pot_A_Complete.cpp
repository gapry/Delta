// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#include "Pot_A_Complete.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "../Common/Finder.h"

APot_A_Complete::APot_A_Complete() {
  {
    static const TCHAR* const Path = TEXT("/Script/GeometryCollectionEngine.GeometryCollection'/Game/Delta/Destructible/"
                                          "GC_SM_Pot_A_Complete1.GC_SM_Pot_A_Complete1'");
    DELTA_SET_GEOMETRY_COLLECTION(GeometryCollectionComponent, Path);
  }

  {
    GeometryCollectionComponent->bUseSizeSpecificDamageThreshold = false;
    GeometryCollectionComponent->SetGenerateOverlapEvents(true);
    GeometryCollectionComponent->SetNotifyRigidBodyCollision(false);

    GeometryCollectionComponent->SetCollisionProfileName(TEXT("Custom"));
    GeometryCollectionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    GeometryCollectionComponent->SetCollisionObjectType(ECollisionChannel::ECC_Destructible);
    GeometryCollectionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
    GeometryCollectionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
    GeometryCollectionComponent->UpdateCollisionProfile();
  }
}
