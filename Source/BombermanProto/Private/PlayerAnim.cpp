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

	//1. 소유폰을 얻어와야 한다.
	auto ownerPawn = TryGetPawnOwner();
	//2. Player 가 필요하다.
	auto player = Cast<ABBMCharacter>(ownerPawn);
	if (player)
	{
		//3. forward vector 가 필요하다.
		FVector forward = player->GetActorForwardVector();
		//4. 속도가 필요하다.
		FVector velocity = player->GetVelocity();
		//5. 나를 소유하고 있는 TPSPlayer의 속도와 forward 내적 값을 할당하고 싶다.
		speed = FVector::DotProduct(forward, velocity);

		//direction 값을 할당하고싶다.
		FVector right = player->GetActorRightVector();
		direction = FVector::DotProduct(right, velocity);
	}
	
	
}

void UPlayerAnim::PlayFlyAnimation()
{
	Montage_Play(FlyMontage);
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FlyMontage->GetName());
}
