// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerUpItem.h"
#include <Components/BoxComponent.h>
#include "BBMCharacter.h"
#include "BombBlast.h"
#include "NBox.h"
#include <Kismet/GameplayStatics.h>


// Sets default values
APowerUpItem::APowerUpItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxcomp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxcomp"));
	RootComponent = boxcomp;
	boxcomp->SetBoxExtent(FVector(50));
	boxcomp->SetCollisionProfileName(TEXT("item"));

	plane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("plane"));
	plane->SetRelativeRotation(FRotator(0,0,20));
	plane->SetRelativeScale3D(FVector(0.75f));
	plane->SetupAttachment(RootComponent);
	plane->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ConstructorHelpers::FObjectFinder<UStaticMesh>tempmesh(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Plane.Plane'"));
	if (tempmesh.Succeeded())
	{
		plane->SetStaticMesh(tempmesh.Object);
	}

	ConstructorHelpers::FObjectFinder<UMaterial>tempmat(TEXT("/Script/Engine.Material'/Game/TJ/M_PowerUP.M_PowerUP'"));
	if (tempmat.Succeeded())
	{
		plane->SetMaterial(0,tempmat.Object);
	}

	ItemGetSound = CreateDefaultSubobject<USoundWave>(TEXT("ItemGetSound"));

	ConstructorHelpers::FObjectFinder<USoundWave> ItemGetSoundAsset(TEXT("/Script/Engine.SoundWave'/Game/SOUND/item-get.item-get'"));
	if (ItemGetSoundAsset.Succeeded())
	{
		ItemGetSound = ItemGetSoundAsset.Object;
	}

	bReplicates = true;
	SetReplicateMovement(true);
}

// Called when the game starts or when spawned
void APowerUpItem::BeginPlay()
{
	Super::BeginPlay();
	
	//태정이형 여길봐
	boxcomp->OnComponentBeginOverlap.AddDynamic(this, &APowerUpItem::OnComponentBeginOverlap);

	auto pc = GetWorld()->GetFirstPlayerController();
	if (pc)
	{
		SetOwner(pc);
	}
}

// Called every frame
void APowerUpItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APowerUpItem::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABBMCharacter* player = Cast<ABBMCharacter>(OtherActor);
	if (player && player->GetController() == nullptr)
	{
		return;
	}
	ABombBlast* blast = Cast<ABombBlast>(UGameplayStatics::GetActorOfClass(GetWorld(),ABombBlast::StaticClass()));
	ANBox* box = Cast<ANBox>(OtherActor);

	if (player != nullptr)
	{
		//수정완료
		player->PowerUp();
		UE_LOG(LogTemp,Warning,TEXT("powerup"));
		Destroy();
		if (ItemGetSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, ItemGetSound, GetActorLocation());
		}
	}
	/*else if (box != nullptr)
	{
		return;
	}*/
// 	else
// 	{
// 		Ishit--;
// 		if (Ishit == 0)
// 		{
// 			Destroy();
// 			UE_LOG(LogTemp, Warning, TEXT("ItemDestroy!!!!"));
// 
// 		}
// 	}
}



