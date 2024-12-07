// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnim.generated.h"

/**
 * 
 */
UCLASS()
class BOMBERMANPROTO_API UPlayerAnim : public UAnimInstance
{
	GENERATED_BODY()
		UPlayerAnim();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Settings")
	float speed = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Settings")
	float direction = 0;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	//ÆøÅº¿¡ ¸Â¾ÒÀ»¶§ ³¯¾Æ°¡´Â ¸ùÅ¸ÁÖ
	UPROPERTY(EditDefaultsOnly,Category="AnimMontage")
	class UAnimMontage* FlyMontage;

	void PlayFlyAnimation();
};
