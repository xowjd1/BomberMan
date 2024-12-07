// Fill out your copyright notice in the Description page of Project Settings.


#include "meteoWidget.h"
#include "Meteo.h"
#include <Components/BoxComponent.h>
#include <Kismet/GameplayStatics.h>

// Sets default values
AmeteoWidget::AmeteoWidget()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	RootComponent = BoxComp;
	BoxComp->SetBoxExtent(FVector(28,26.6f,3));

	plane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("plane"));
	plane->SetRelativeScale3D(FVector(0.5f, 0.5f, 1));
	plane->SetupAttachment(RootComponent);
	plane->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ConstructorHelpers::FObjectFinder<UStaticMesh>tempmesh(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Plane.Plane'"));
	if (tempmesh.Succeeded())
	{
		plane->SetStaticMesh(tempmesh.Object);
	}
	ConstructorHelpers::FObjectFinder<UMaterial>tempmat(TEXT("/Script/Engine.Material'/Game/CR/warning_Mat.warning_Mat'"));
	if (tempmat.Succeeded())
	{
		plane->SetMaterial(0, tempmat.Object);
	}

}

// Called when the game starts or when spawned
void AmeteoWidget::BeginPlay()
{
	Super::BeginPlay();


}

// Called every frame
void AmeteoWidget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


