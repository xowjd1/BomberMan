// Fill out your copyright notice in the Description page of Project Settings.


#include "SpeedUp.h"
#include <Components/BoxComponent.h>
#include "BBMCharacter.h"
#include <Kismet/GameplayStatics.h>
#include <Sound/SoundWave.h>

// Sets default values
ASpeedUp::ASpeedUp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxcomp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxcomp"));
	RootComponent = boxcomp;
	boxcomp->SetBoxExtent(FVector(50));
	boxcomp->SetCollisionProfileName(TEXT("item"));

	SpeedItem = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpeedItem"));
	SpeedItem->SetRelativeRotation(FRotator(0, 0, 20));
	SpeedItem->SetRelativeScale3D(FVector(0.75f));
	SpeedItem->SetupAttachment(RootComponent);
	SpeedItem->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ConstructorHelpers::FObjectFinder<UStaticMesh>tempmesh(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Plane.Plane'"));
	if (tempmesh.Succeeded())
	{
		SpeedItem->SetStaticMesh(tempmesh.Object);
	}

	ConstructorHelpers::FObjectFinder<UMaterial>tempmat(TEXT("/Script/Engine.Material'/Game/TJ/M_SpeedUP.M_SpeedUp'"));
	if (tempmat.Succeeded())
	{
		SpeedItem->SetMaterial(0, tempmat.Object);
	}


	ItemGetSound = CreateDefaultSubobject<USoundWave>(TEXT("ItemGetSound"));

	ConstructorHelpers::FObjectFinder<USoundWave> ItemGetSoundAsset(TEXT("/Script/Engine.SoundWave'/Game/SOUND/item-get.item-get'"));
	if (ItemGetSoundAsset.Succeeded())
	{
		ItemGetSound = ItemGetSoundAsset.Object;
	}

	bReplicates = true;

}

// Called when the game starts or when spawned
void ASpeedUp::BeginPlay()
{
	Super::BeginPlay();

	boxcomp->OnComponentBeginOverlap.AddDynamic(this, &ASpeedUp::OnComponentBeginOverlap);

}

// Called every frame
void ASpeedUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpeedUp::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABBMCharacter* player = Cast<ABBMCharacter>(OtherActor);
	if (player)
	{
		player->SpeedUp();
		Destroy();
		if (ItemGetSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, ItemGetSound, GetActorLocation());
		}
	}
}

