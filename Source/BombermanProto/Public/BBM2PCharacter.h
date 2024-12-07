// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "BBM2PCharacter.generated.h"

class UInputMappingContext;
class UInputAction;


UCLASS()
class BOMBERMANPROTO_API ABBM2PCharacter : public ACharacter
{
	GENERATED_BODY()

public:
//	// Sets default values for this character's properties
//	ABBM2PCharacter();
//
//protected:
//	// Called when the game starts or when spawned
//	virtual void BeginPlay() override;
//
//public:	
//	// Called every frame
//	virtual void Tick(float DeltaTime) override;
//
//	// Called to bind functionality to input
//	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
//
//	//enhanced Input
//	UPROPERTY(EditAnywhere, Category = Input)
//	UInputMappingContext* defaultContext;
//	UPROPERTY(EditAnywhere, Category = Input)
//	UInputAction* MovementAction;
//	UPROPERTY(EditAnywhere, Category = Input)
//	UInputAction* SetBombAction;
//	//캐릭터
//	//캐릭터 움직임
//	void Move(const FInputActionValue& value);
//
//	//폭탄
//	//폭탄 놓는 함수 (space버튼)
//	void SetBomb();
//
//	//폭탄에 맞았을 경우
//	//폭탄에 맞았을 경우 실행되는 함수(푱날아가게)
//	void HitBomb();
//	//폭탄에 맞았을 때 날아가는 시간 (CreateTime값에 따라 시간초 조정 가능)
//	float FlyCurrentTime;
//	float FlyCreateTime = 3;
//	//폭탄에 맞았을 때 날아가는 속도
//	UPROPERTY(EditAnywhere,Category=Fly)
//	float FlySpeed=700;
//	//폭탄에 맞아서 날아가야되는지?
//	UPROPERTY(EditAnywhere,Category=Fly)
//	bool IsFly = false;
//	//날아갈 방향
//	FVector FlyDirection = FVector::UpVector+FVector::RightVector;
//	//몽타주 플레이
//	class UPlayerAnim* FlyMontage;
//
//	UPROPERTY(VisibleAnywhere,Category=Fly)
//   class UNiagaraComponent* blast_Particle;

};
