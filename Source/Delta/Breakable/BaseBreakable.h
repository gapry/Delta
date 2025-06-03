// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Physics/Experimental/ChaosEventType.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/CapsuleComponent.h"
#include "../Treasure/BaseTreasure.h"
#include "../Interface/HitInterface.h"
#include "BaseBreakable.generated.h"

#define DELTA_BREAKABLE_ENABLE_DEBUG_HIT 0

class UGeometryCollectionComponent;
class USoundBase;

UCLASS()
class DELTA_API ABaseBreakable : public AActor, public IHitInterface {
  GENERATED_BODY()

public:
  ABaseBreakable();

  virtual void GetHit(const FVector& ImpactPoint) override;

  UFUNCTION()
  void OnBreakEvent(const FChaosBreakEvent& BreakEvent);

  void AddTreasureClass(TSubclassOf<ABaseTreasure> InsertedTreasureClass);

protected:
  virtual void BeginPlay() override;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Geometry Collection")
  TObjectPtr<UGeometryCollectionComponent> GeometryCollectionComponent{nullptr};

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound")
  TObjectPtr<USoundBase> BreakSound{nullptr};

  UPROPERTY(EditAnywhere, Category = "Breakable Properties")
  TArray<TSubclassOf<class ABaseTreasure>> TreasureClasses;

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  TObjectPtr<UCapsuleComponent> Capsule{nullptr};

  float LifeSpan{5.5f};
};
