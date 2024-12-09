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

    //���� �ݸ��� ������Ʈ ����
    blastComp = CreateDefaultSubobject<USphereComponent>(TEXT("blastComp"));
    //��Ʈ������Ʈ�� ���� �ݸ������� ����
    RootComponent = blastComp;
    //���� �ݸ��� �����ϰ� ����
    blastComp->SetRelativeScale3D(FVector(1.0f));
    blastComp->SetCollisionProfileName(TEXT("blast"));




    //���� ���̾ư���
    blast_Particle = CreateDefaultSubobject<UNiagaraComponent>(TEXT("blast_Particle"));


    ConstructorHelpers::FObjectFinder<UNiagaraSystem>tempNiagara(TEXT("/Script/Niagara.NiagaraSystem'/Game/TJ/Particles/Fireball/Blast_Particle.Blast_Particle'"));

    if (tempNiagara.Succeeded())
    {
        UNiagaraSystem* NiagaraSystem = tempNiagara.Object;

        // NiagaraSystem�� Fuse_Particle ������Ʈ�� �Ҵ�
        blast_Particle->SetAsset(NiagaraSystem);

        // ������Ʈ�� ���忡 �߰�
        blast_Particle->AttachToComponent(blastComp, FAttachmentTransformRules::KeepRelativeTransform);
        blast_Particle->SetRelativeLocation(FVector(0));
        //�ݸ����� ������� �ʰ� ��������Ʈ �����ϰ� ����
        blast_Particle->SetRelativeScale3D(FVector(1.5f));

    }
    bReplicates = true; //��������� �Ҵ�.
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
//    //box�� ������ �� ��츦 ���� �ڽ� ã�Ƴ���
//    box = Cast<ANBox>(UGameplayStatics::GetActorOfClass(GetWorld(), ANBox::StaticClass()));
//    
//    //��
//    me = Cast<ABombBlast>(GetOwner());
//
//    //�������� �����
//    //���� Ȯ���� ���Ѵ�.
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
  

    // ���� �ð� �帣�� �ϱ�
    currentTime += GetWorld()->DeltaTimeSeconds;

    // ����ð��� createTime ���� Ŀ���� ����Ʈ ����
    if (currentTime > createTime)
    {
        Destroy();
    }
 


}

void ABombBlast::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    //�������Ǵ� ���͵� ���� / ��ź ���� ó�� ( ���� �ڽ� �� �׵θ� ���� ���� ��������� )
    ABomb* BombOwner = Cast<ABomb>(GetOwner());
    ABBMCharacter* player = Cast<ABBMCharacter>(OtherActor);
    //�������� �����
    APowerUpItem* powerup = Cast<APowerUpItem>(OtherActor);
    ANBox* boxowner = Cast<ANBox>(GetOwner());

    if (OtherActor != nullptr && !OtherActor->IsA<ABomb>())
    {
            //�������� �����
            if (OtherActor->IsA<ANBox>())
            {
                //27�ۼ�Ʈ Ȯ����
                if (RandomSpawn < 50)
                {
                    //�������� �����ϰڴ�.
                    box->SpawnItem(Randomitem, OtherActor->GetActorLocation());
                    UE_LOG(LogTemp, Warning, TEXT("Spawn!!!!"));
                }
                OtherActor->Destroy();
                //Ȯ���� �缳��
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


