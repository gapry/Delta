// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#pragma once

#include "CoreMinimal.h"
#include "ItemState.h"
#include "Item.generated.h"

class USphereComponent;
class UNiagaraComponent;

UCLASS()
class DELTA_API AItem : public AActor {
  GENERATED_BODY()

public:
  AItem();

  virtual void OnConstruction(const FTransform& Transform) override;

  virtual void Tick(float DeltaTime) override;

  void SetLocation(const FVector& NewLocation);
  void SetRotation(const FRotator& NewRotation);
  void UpdateForwardDirection();

protected:
  virtual void BeginPlay() override;

  virtual void PostInitializeComponents() override;
  virtual void PostInitializeRootComponent();
  virtual void PostInitializeStaticMeshComponent();

  UFUNCTION()
  virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent,
                                    AActor*              OtherActor,
                                    UPrimitiveComponent* OtherComp,
                                    int32                OtherBodyIndex,
                                    bool                 bFromSweep,
                                    const FHitResult&    SweepResult);

  UFUNCTION()
  virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

  UFUNCTION(BlueprintCallable, Category = "Item")
  float GetSineOscillationOffset() const;

  UFUNCTION(BlueprintCallable, Category = "Item")
  float GetCosineOscillationOffset() const;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
  TObjectPtr<UStaticMeshComponent> StaticMeshComponent{nullptr};

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item")
  TObjectPtr<USphereComponent> SphereComponent{nullptr};

  UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item", meta = (AllowPrivateAccess = "true"))
  float MovementRate{50.f};

  UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item", meta = (AllowPrivateAccess = "true"))
  float RotationRate{45.f};

  UPROPERTY(EditAnywhere)
  TObjectPtr<UNiagaraComponent> EmbersEffect{nullptr};

  EItemState ItemState = EItemState::EIS_Hovering;

private:
  UPROPERTY(VisibleAnywhere, Category = "Item")
  FVector ForwardDirection{FVector::ForwardVector};

  UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item", meta = (AllowPrivateAccess = "true"))
  float Amplitude{0.5f};

  UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item", meta = (AllowPrivateAccess = "true"))
  float Frequency{0.5f};

  UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Item", meta = (AllowPrivateAccess = "true"))
  float RunningTime{0.f};
};
