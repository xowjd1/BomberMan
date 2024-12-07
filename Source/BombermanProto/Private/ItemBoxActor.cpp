// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBoxActor.h"
#include "ItemOne.h"
#include "kismet/GameplayStatics.h"

// Sets default values
AItemBoxActor::AItemBoxActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

}

// Called when the game starts or when spawned
void AItemBoxActor::BeginPlay()
{
	Super::BeginPlay();
	
	RandItem();
}

// Called every frame
void AItemBoxActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItemBoxActor::RandItem()
{
	// 아이템을 스포너의 위치에 스폰한다
	FVector SpawnLocation = FVector(GetActorLocation());

	AActor* SpawnActor = GetWorld()->SpawnActor<AActor>(ItemClass, SpawnLocation, FRotator::ZeroRotator);
}

