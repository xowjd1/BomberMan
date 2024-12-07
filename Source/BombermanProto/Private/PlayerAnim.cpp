// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnim.h"
#include "BBMCharacter.h"

UPlayerAnim::UPlayerAnim()
{
	ConstructorHelpers::FObjectFinder<UAnimMontage>FlyanimMontage(TEXT("/Script/Engine.AnimMontage'/Game/Bluprints/Animations/Montage/MT_Falling.MT_Falling'"));
	if (FlyanimMontage.Succeeded())
	{
		FlyMontage = FlyanimMontage.Object;
	}
}

void UPlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	//1. �������� ���;� �Ѵ�.
	auto ownerPawn = TryGetPawnOwner();
	//2. Player �� �ʿ��ϴ�.
	auto player = Cast<ABBMCharacter>(ownerPawn);
	if (player)
	{
		//3. forward vector �� �ʿ��ϴ�.
		FVector forward = player->GetActorForwardVector();
		//4. �ӵ��� �ʿ��ϴ�.
		FVector velocity = player->GetVelocity();
		//5. ���� �����ϰ� �ִ� TPSPlayer�� �ӵ��� forward ���� ���� �Ҵ��ϰ� �ʹ�.
		speed = FVector::DotProduct(forward, velocity);

		//direction ���� �Ҵ��ϰ�ʹ�.
		FVector right = player->GetActorRightVector();
		direction = FVector::DotProduct(right, velocity);
	}
	
	
}

void UPlayerAnim::PlayFlyAnimation()
{
	Montage_Play(FlyMontage);
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FlyMontage->GetName());
}
