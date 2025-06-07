// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#include "AttributeComponent.h"

UAttributeComponent::UAttributeComponent() {
  PrimaryComponentTick.bCanEverTick = true;
}

void UAttributeComponent::BeginPlay() {
  Super::BeginPlay();
}

void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
  Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UAttributeComponent::ReceiveDamage(const float DamageAmount) {
  SetHealth(GetHealth() - DamageAmount);
}

void UAttributeComponent::SetHealth(const float NewHealth) {
  CurrentHealth = FMath::Clamp(NewHealth, 0.0f, MaximumHealth);
}

float UAttributeComponent::GetHealth() const {
  return CurrentHealth;
}

float UAttributeComponent::GetHealthPercent() const {
  return CurrentHealth / MaximumHealth;
}
