// Fill out your copyright notice in the Description page of Project Settings.


#include "NetPlayerController.h"
#include <Kismet/GameplayStatics.h>
#include "NStartBoxSpawn.h"
#include "NBoxSpawnActor.h"

void ANetPlayerController::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("PlayerController Beginplay"));

	if (HasAuthority())
	{
		gm = GetWorld()->GetAuthGameMode<ABombermanProtoGameModeBase>();
		auto boxSpawner = UGameplayStatics::GetActorOfClass(GetWorld(), ANStartBoxSpawn::StaticClass());
		UE_LOG(LogTemp, Warning, TEXT("11111111111111111111111"));
		if (boxSpawner)
		{
			UE_LOG(LogTemp, Warning, TEXT("ANStartBoxSpawn SetOwner ------------------------"));
			boxSpawner->SetOwner(this);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("not own"));
	}

	if (HasAuthority())
	{
		auto nboxSpawner = UGameplayStatics::GetActorOfClass(GetWorld(), ANBoxSpawnActor::StaticClass());
		UE_LOG(LogTemp, Warning, TEXT("2222222222222222222222"));
		if (nboxSpawner)
		{
			UE_LOG(LogTemp, Warning, TEXT(" ANBoxSpawnActor SetOwner ------------------------"));0
			nboxSpawner->SetOwner(this);
		}
	}
}
