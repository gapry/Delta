// Copyright (c) 2025 Gapry.
// Licensed under the MIT License.
// See LICENSE file in the project root for full license information.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "../../Character/State/CharacterState.h"
#include "../../Character/State/ActionState.h"
#include "../../Character/BaseCharacter.h"
#include "EchoCharacter.generated.h"

class USkeletalMeshComponent;
class UCapsuleComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UEnhancedInputLocalPlayerSubsystem;
class UGroomComponent;
class AItem;
class UAnimMontage;

UCLASS()
class DELTA_API AEchoCharacter : public ABaseCharacter {
  GENERATED_BODY()

public:
  AEchoCharacter();

  virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

  virtual void Jump() override;
  void         Move(const FInputActionValue& Value);
  void         Look(const FInputActionValue& Value);
  void         Equip(const FInputActionValue& Value);
  void         Attack(const FInputActionValue& Value);

  void   SetOverlappingItem(AItem* const Item);
  AItem* GetOverlappingItem() const;

  ECharacterState GetCharacterState() const;
  void            SetCharacterState(ECharacterState NewState);

  void PlayEquipUnequipMontage(const FName SectionName) const;

  void AttackAnimNotify();

  void Arm();
  void Disarm();
  void FinishEquipping();

  bool CanArm() const;
  bool CanDisarm() const;

  virtual bool CanAttack() override;

  virtual void GetHit(const FVector& ImpactPoint) override;

protected:
  virtual void BeginPlay() override;

  virtual void Die() override;

  virtual void PlayAttackMontage() override;

private:
  APlayerController*                  GetPlayerController() const;
  UEnhancedInputLocalPlayerSubsystem* GetSubsystem() const;

  ECharacterState CharacterState = ECharacterState::ECS_Unequipped;
  EActionState    ActionState    = EActionState::EAS_Unoccupied;

  TObjectPtr<UInputMappingContext> InputMappingContext;
  TObjectPtr<UInputAction>         MoveAction;
  TObjectPtr<UInputAction>         LookAction;
  TObjectPtr<UInputAction>         JumpAction;
  TObjectPtr<UInputAction>         EquipAction;
  TObjectPtr<UInputAction>         AttackAction;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hair", meta = (AllowPrivateAccess = "true"))
  TSoftObjectPtr<UGroomComponent> HairComponent;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Eye", meta = (AllowPrivateAccess = "true"))
  TSoftObjectPtr<UGroomComponent> EyebrowsComponent;

  UPROPERTY(VisibleInstanceOnly, Category = "Item")
  TObjectPtr<AItem> OverlappingItem;

  UPROPERTY(EditDefaultsOnly, Category = "Montage")
  TObjectPtr<UAnimMontage> EquipUnequipMontage{nullptr};
};
