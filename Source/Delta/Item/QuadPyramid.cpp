// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#include "QuadPyramid.h"
#include "Components/SphereComponent.h"
#include "../Common/Finder.h"

AQuadPyramid::AQuadPyramid() {
  {
    static const TCHAR* const Path =
      TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_QuadPyramid'");
    DELTA_SET_STATIC_MESH(StaticMeshComponent, Path);
  }

  {
    SphereComponent->SetSphereRadius(105.f);
  }

  {
    StaticMeshComponent->SetCollisionProfileName(TEXT("Custom"));
    StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    StaticMeshComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
    StaticMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
    StaticMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera,
                                                       ECollisionResponse::ECR_Ignore);
    StaticMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,
                                                       ECollisionResponse::ECR_Overlap);
    StaticMeshComponent->UpdateCollisionProfile();
  }
}

void AQuadPyramid::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent,
                                        AActor*              OtherActor,
                                        UPrimitiveComponent* OtherComp,
                                        int32                OtherBodyIndex,
                                        bool                 bFromSweep,
                                        const FHitResult&    SweepResult) {
  const FString& OtherActorName = FString("Begining Overlap with: ") + OtherActor->GetName();
  if (GEngine) {
    GEngine->AddOnScreenDebugMessage(1, 30.f, FColor::Red, OtherActorName);
  }
}

void AQuadPyramid::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent,
                                      AActor*              OtherActor,
                                      UPrimitiveComponent* OtherComp,
                                      int32                OtherBodyIndex) {
  const FString OtherActorName = FString("Ending Overlap with: ") + OtherActor->GetName();
  if (GEngine) {
    GEngine->AddOnScreenDebugMessage(1, 30.f, FColor::Red, OtherActorName);
  }
}