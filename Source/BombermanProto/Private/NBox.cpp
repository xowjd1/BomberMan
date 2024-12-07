// Fill out your copyright notice in the Description page of Project Settings.


#include "NBox.h"
#include <Components/BoxComponent.h>
#include "BombBlast.h"
#include "PowerUpItem.h"
#include <Kismet/GameplayStatics.h>
#include "SpeedUp.h"
#include "UpgradeBombCount.h"

// Sets default values
ANBox::ANBox()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // 박스 컴포넌트 생성
    BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
    RootComponent = BoxComp;
    BoxComp->SetBoxExtent(FVector(8));
    // 콜리전에 이름 붙여주기
    BoxComp->SetCollisionProfileName(TEXT("box"));

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
    ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(TEXT("/Script/Engine.StaticMesh'/Game/08_3DXR/SM_Box.SM_Box'"));

    if (TempMesh.Succeeded())
    {
        BodyMesh->SetStaticMesh(TempMesh.Object);
    }

    bReplicates = true;
    SetReplicateMovement(true);

}

// Called when the game starts or when spawned
void ANBox::BeginPlay()
{
    Super::BeginPlay();

    BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ANBox::OnComponentBeginOverlap);

}

// Called every frame
void ANBox::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // 위에서 아래로 내려오게
    FVector p0 = GetActorLocation();
    FVector vt = -GetActorUpVector() * speed * DeltaTime;
    FVector P = p0 + vt;
    SetActorLocation(P, true);
   

    
}

void ANBox::DestroyBox()
{
    isDestroy = true;
    Destroy();
}

void ANBox::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    ABombBlast* blast = Cast<ABombBlast>(OtherActor);
    if (blast != nullptr)
    {
        bStop = true;
    }
}

//태정이형 여길봐
void ANBox::SpawnItem(int val, FVector location)
{
  
    //확률에 따라서 함수 내에서 spawn할 아이템을 나눔.
    if (val < 33)
    {
        APowerUpItem* powerup = Cast<APowerUpItem>(UGameplayStatics::GetActorOfClass(GetWorld(), APowerUpItem::StaticClass()));
        GetWorld()->SpawnActor<APowerUpItem>(PowerUpItem, location, GetActorRotation());
        UE_LOG(LogTemp, Warning, TEXT("powerupspawn"));
    }
    else if (val < 66)
    {
        ASpeedUp* speedup = Cast<ASpeedUp>(UGameplayStatics::GetActorOfClass(GetWorld(), ASpeedUp::StaticClass()));
        GetWorld()->SpawnActor<ASpeedUp>(SpeedUpItem, location, GetActorRotation());
        UE_LOG(LogTemp, Warning, TEXT("speedupspawn"));
    }
    else
    {
        AUpgradeBombCount* bombup = Cast<AUpgradeBombCount>(UGameplayStatics::GetActorOfClass(GetWorld(), AUpgradeBombCount::StaticClass()));
        GetWorld()->SpawnActor<AUpgradeBombCount>(BombUpItem, location, GetActorRotation());
        UE_LOG(LogTemp, Warning, TEXT("bombupspawn"));
    }
}

