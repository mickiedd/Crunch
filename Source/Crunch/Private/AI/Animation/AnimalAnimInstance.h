// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AnimalAnimInstance.generated.h"

/**
 *
 */
UCLASS()
class UAnimalAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeInitializeAnimation() override;
	// Native update override point. It is usually a good idea to simply gather data in this step and
	// for the bulk of the work to be done in NativeThreadSafeUpdateAnimation.
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	// Native thread safe update override point. Executed on a worker thread just prior to graph update
	// for linked anim instances, only called when the hosting node(s) are relevant
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable, Category = "Animation", meta = (BlueprintThreadSafe))
	bool IsMoving() const { return Speed != 0; }

	UFUNCTION(BlueprintCallable, Category = "Animation", meta = (BlueprintThreadSafe))
	bool IsRunning() const { return Speed > 150.f; }

	UPROPERTY()
	class ACharacter* OwnerCharacter;

	UPROPERTY()
	class UCharacterMovementComponent* OwnerMovementComp;

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	float Speed;

};
