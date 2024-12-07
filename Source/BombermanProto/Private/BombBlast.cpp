// Fill out your copyright notice in the Description page of Project Settings.


#include "BombBlast.h"
#include <../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraComponent.h>
#include "Bomb.h"
#include "Net/UnrealNetwork.h"
#include <Components/SphereComponent.h>
#include <Kismet/KismetSystemLibrary.h>
#include "BBMCharacter.h"
#include <Kismet/GameplayStatics.h>
#include "NBox.h"
#include <Engine/EngineTypes.h>
#include "PowerUpItem.h"
#include "GameFramework/Actor.h"

// Sets default values
ABombBlast::ABombBlast()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    //폭발 콜리전 컴포넌트 생성
    blastComp = CreateDefaultSubobject<USphereComponent>(TEXT("blastComp"));
    //루트컴포넌트를 폭발 콜리전으로 설정
    RootComponent = blastComp;
    //폭발 콜리전 스케일값 조정
    blastComp->SetRelativeScale3D(FVector(1.0f));
    blastComp->SetCollisionProfileName(TEXT("blast"));




    //폭발 나이아가라
    blast_Particle = CreateDefaultSubobject<UNiagaraComponent>(TEXT("blast_Particle"));


    ConstructorHelpers::FObjectFinder<UNiagaraSystem>tempNiagara(TEXT("/Script/Niagara.NiagaraSystem'/Game/TJ/Particles/Fireball/Blast_Particle.Blast_Particle'"));

    if (tempNiagara.Succeeded())
    {
        UNiagaraSystem* NiagaraSystem = tempNiagara.Object;

        // NiagaraSystem을 Fuse_Particle 컴포넌트에 할당
        blast_Particle->SetAsset(NiagaraSystem);

        // 컴포넌트를 월드에 추가
        blast_Particle->AttachToComponent(blastComp, FAttachmentTransformRules::KeepRelativeTransform);
        blast_Particle->SetRelativeLocation(FVector(0));
        //콜리전에 어색하지 않게 폭발이펙트 스케일값 조정
        blast_Particle->SetRelativeScale3D(FVector(1.5f));

    }
    bReplicates = true; //복제기능을 켠다.
    SetReplicateMovement(true);
}

// Called when the game starts or when spawned
void ABombBlast::BeginPlay()
{
    Super::BeginPlay();

    if (GetOwner() == nullptr)
    {
        return;
    }
    auto owner = Cast<ABBMCharacter>(GetOwner()->GetOwner());
    if (owner)
    {
       // PlayBomb(owner->length);
    }
}

//void ABombBlast::PlayBomb_Implementation(ABBMCharacter* player)
//{
//    auto pc = GetWorld()->GetFirstPlayerController();
//   // UE_LOG(LogTemp, Warning, TEXT("PlayBomb NetMode: %d, length : %f"), pc->GetNetMode(), player->length);
//
//    if (HasAuthority())
//    {
//        blastComp->OnComponentBeginOverlap.AddDynamic(this, &ABombBlast::OnComponentBeginOverlap);
//    }
//
//	/*StartLocation = GetActorLocation();
//	FVector EndLocation = StartLocation + (GetActorForwardVector() * player->length);
//
//	TArray<AActor*> EmptyActorsToIgnore;
//	FHitResult HitResult;
//
//	TArray<TEnumAsByte<EObjectTypeQuery>> queryArray;
//	queryArray.Add(EObjectTypeQuery::ObjectTypeQuery1);*/
//
//
//    //box만 오버랩 될 경우를 위해 박스 찾아놓기
//    box = Cast<ANBox>(UGameplayStatics::GetActorOfClass(GetWorld(), ANBox::StaticClass()));
//    
//    //나
//    me = Cast<ABombBlast>(GetOwner());
//
//    //태정이형 여길봐
//    //랜덤 확률을 정한다.
//	RandomSpawn = FMath::RandRange(1, 100);
//	Randomitem = FMath::RandRange(1, 100);
//
//}

// Called every frame
void ABombBlast::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

  //  if (stop == false)
  //  {
  //      //AddActorWorldOffset(DeltaTime * speed * GetActorForwardVector(), true, nullptr, ETeleportType::None);
	/*auto pc = GetWorld()->GetFirstPlayerController();
	if (pc == nullptr)
	{
		return;
	}*/
  //      auto player = Cast<ABBMCharacter>(GetOwner());
  //      UE_LOG(LogTemp, Warning, TEXT("PlayBomb NetMode: %d, length : %f"), pc->GetNetMode(), player->length);
  //      //if (A.Length() >= player->length)
		//{
		//	speed = 0;
  //      }
  //  }
  //  else
  //      speed = 0;
  

    // 현재 시간 흐르게 하기
    currentTime += GetWorld()->DeltaTimeSeconds;

    // 현재시간이 createTime 보다 커지면 이펙트 삭제
    if (currentTime > createTime)
    {
        Destroy();
    }
 


}

void ABombBlast::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    //오버랩되는 액터들 제거 / 폭탄 예외 처리 ( 고정 박스 및 테두리 추후 포함 시켜줘야함 )
    ABomb* BombOwner = Cast<ABomb>(GetOwner());
    ABBMCharacter* player = Cast<ABBMCharacter>(OtherActor);
    //태정이형 여길봐
    APowerUpItem* powerup = Cast<APowerUpItem>(OtherActor);
    ANBox* boxowner = Cast<ANBox>(GetOwner());

    if (OtherActor != nullptr && !OtherActor->IsA<ABomb>())
    {
            //태정이형 여길봐
            if (OtherActor->IsA<ANBox>())
            {
                //27퍼센트 확률로
                if (RandomSpawn < 50)
                {
                    //아이템을 스폰하겠다.
                    box->SpawnItem(Randomitem, OtherActor->GetActorLocation());
                    UE_LOG(LogTemp, Warning, TEXT("Spawn!!!!"));
                }
                OtherActor->Destroy();
                //확률값 재설정
                RandomSpawn = FMath::RandRange(1, 100);
                Randomitem = FMath::RandRange(1, 100);
            }
    }
        else if (powerup != nullptr)
        {
            return;
        }
        else
        {
            OtherActor->Destroy();
        }
   }


