// Fill out your copyright notice in the Description page of Project Settings.


#include "UpgradeBombCount.h"
#include "BBMCharacter.h"
#include <Kismet/GameplayStatics.h>
#include <Sound/SoundWave.h>

// Sets default values
AUpgradeBombCount::AUpgradeBombCount()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxcomp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxcomp"));
	RootComponent = boxcomp;
	boxcomp->SetBoxExtent(FVector(50));
	boxcomp->SetCollisionProfileName(TEXT("item"));

	bombItem = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("bombItem"));
	bombItem->SetRelativeRotation(FRotator(0, 0, 20));
	bombItem->SetRelativeScale3D(FVector(0.75f));
	bombItem->SetupAttachment(RootComponent);
	bombItem->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ConstructorHelpers::FObjectFinder<UStaticMesh>tempmesh(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Plane.Plane'"));
	if (tempmesh.Succeeded())
	{
		bombItem->SetStaticMesh(tempmesh.Object);
	}

	ConstructorHelpers::FObjectFinder<UMaterial>tempmat(TEXT("/Script/Engine.Material'/Game/TJ/M_BombUp.M_BombUp'"));
	if (tempmat.Succeeded())
	{
		bombItem->SetMaterial(0, tempmat.Object);
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
void AUpgradeBombCount::BeginPlay()
{
	Super::BeginPlay();

	boxcomp->OnComponentBeginOverlap.AddDynamic(this, &AUpgradeBombCount::OnComponentBeginOverlap);

}

// Called every frame
void AUpgradeBombCount::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AUpgradeBombCount::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABBMCharacter* player = Cast<ABBMCharacter>(OtherActor);
	if (player)
	{
		player->UpgradeBombCount();
		Destroy();
		if (ItemGetSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, ItemGetSound, GetActorLocation());
		}
	}
}

