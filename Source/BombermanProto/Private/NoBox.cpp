// Fill out your copyright notice in the Description page of Project Settings.


#include "NoBox.h"
#include <Components/BoxComponent.h>

// Sets default values
ANoBox::ANoBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// �ڽ� ������Ʈ ����
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	RootComponent = BoxComp;
	BoxComp->SetBoxExtent(FVector(8));
	// �ݸ����� �̸� �ٿ��ֱ�
	BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	// �޽� ����
	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	BodyMesh->SetupAttachment(RootComponent);
	// �ݸ��� ����
	BodyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);


	// ��ġ ȸ�� ũ��
	BodyMesh->SetRelativeLocation(FVector(20, 0, 20));
	BodyMesh->SetRelativeRotation(FRotator(0, 90, 90));
	BodyMesh->SetRelativeScale3D(FVector(1));

	// �����Ҵ�
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
	// ������ �Ʒ��� ��������
	FVector p0 = GetActorLocation();
	FVector vt = -GetActorUpVector() * speed * DeltaTime;
	FVector P = p0 + vt;
	SetActorLocation(P, true);
}

