// Fill out your copyright notice in the Description page of Project Settings.


#include "MapOut.h"
#include <Components/BoxComponent.h>

// Sets default values
AMapOut::AMapOut()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mapOutMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("mapOutMesh"));
	RootComponent = mapOutMesh;
	mapOutMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	mapOutMesh->SetRelativeScale3D(FVector(1));

	ConstructorHelpers::FObjectFinder<UStaticMesh>tempmesh1(TEXT("/Script/Engine.StaticMesh'/Game/Border/Border_.Border_'"));
	if (tempmesh1.Succeeded())
	{
		mapOutMesh->SetStaticMesh(tempmesh1.Object);
	}

	floorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("floorMesh"));
	floorMesh->SetupAttachment(RootComponent);
/*	floorMesh->SetRelativeLocation(FVector(800, -1000, 0));*/
	floorMesh->SetRelativeScale3D(FVector(1));
	floorMesh->SetCollisionProfileName(TEXT("floor"));
	floorMesh->SetRelativeRotation(FRotator(0, 90, 0));

	ConstructorHelpers::FObjectFinder<UStaticMesh>tempmesh2(TEXT("/Script/Engine.StaticMesh'/Game/T_mapfloorbox/mapfloor.mapfloor'"));
	if (tempmesh2.Succeeded())
	{
		floorMesh->SetStaticMesh(tempmesh2.Object);
	}

	mapboxcomp1 = CreateDefaultSubobject<UBoxComponent>(TEXT("boxcomp1"));
	mapboxcomp1->SetCollisionProfileName(TEXT("iron"));
	mapboxcomp1->SetRelativeLocation(FVector(975, 0, 60));
	mapboxcomp1->SetRelativeRotation(FRotator(0, 90, 0));
	mapboxcomp1->SetRelativeScale3D(FVector(24, 1, 3));
	mapboxcomp1->SetupAttachment(RootComponent);

	mapboxcomp2 = CreateDefaultSubobject<UBoxComponent>(TEXT("boxcomp2"));
	mapboxcomp2->SetCollisionProfileName(TEXT("iron"));
	mapboxcomp2->SetRelativeLocation(FVector(-975, 0, 60));
	mapboxcomp2->SetRelativeScale3D(FVector(24, 1, 3));
	mapboxcomp2->SetRelativeRotation(FRotator(0, 90, 0));
	mapboxcomp2->SetupAttachment(RootComponent);

	mapboxcomp3 = CreateDefaultSubobject<UBoxComponent>(TEXT("boxcomp3"));
	mapboxcomp3->SetCollisionProfileName(TEXT("iron"));
	mapboxcomp3->SetRelativeLocation(FVector(0, -775, 60));
	mapboxcomp3->SetRelativeScale3D(FVector(30, 1, 3));
	mapboxcomp3->SetupAttachment(RootComponent);

	mapboxcomp4 = CreateDefaultSubobject<UBoxComponent>(TEXT("boxcomp4"));
	mapboxcomp4->SetCollisionProfileName(TEXT("iron"));
	mapboxcomp4->SetRelativeLocation(FVector(0, 775, 60));
	mapboxcomp4->SetRelativeScale3D(FVector(30, 1, 3));
	mapboxcomp4->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AMapOut::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AMapOut::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

