// Fill out your copyright notice in the Description page of Project Settings.


#include "NoBox.h"
#include <Components/BoxComponent.h>

// Sets default values
ANoBox::ANoBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// 박스 컴포넌트 생성
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	RootComponent = BoxComp;
	BoxComp->SetBoxExtent(FVector(8));
	// 콜리전에 이름 붙여주기
	BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	// 메쉬 생성
	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	BodyMesh->SetupAttachment(RootComponent);
	// 콜리전 설정
	BodyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);


	// 위치 회전 크기
	BodyMesh->SetRelativeLocation(FVector(20, 0, 20));
	BodyMesh->SetRelativeRotation(FRotator(0, 90, 90));
	BodyMesh->SetRelativeScale3D(FVector(1));

	// 동적할당
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(TEXT("/Script/Engine.StaticMesh'/Game/XBox2/SM_XBOX2.SM_XBOX2'"));

	if (TempMesh.Succeeded())
	{
		BodyMesh->SetStaticMesh(TempMesh.Object);
	}
}

// Called when the game starts or when spawned
void ANoBox::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANoBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// 위에서 아래로 내려오게
	FVector p0 = GetActorLocation();
	FVector vt = -GetActorUpVector() * speed * DeltaTime;
	FVector P = p0 + vt;
	SetActorLocation(P, true);
}

