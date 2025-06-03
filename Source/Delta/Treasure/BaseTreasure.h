// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "../Item/Item.h"
#include "BaseTreasure.generated.h"

class USoundBase;

UCLASS()
class DELTA_API ABaseTreasure : public AItem {
  GENERATED_BODY()

public:
  ABaseTreasure();

  virtual void Tick(float DeltaTime) override;

protected:
  virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent,
                                    AActor*              OtherActor,
                                    UPrimitiveComponent* OtherComp,
                                    int32                OtherBodyIndex,
                                    bool                 bFromSweep,
                                    const FHitResult&    SweepResult) override;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound")
  TObjectPtr<USoundBase> PickupSound{nullptr};

  UPROPERTY(EditAnywhere, Category = "Treasure Properties")
  float Gold{0.0f};
};
