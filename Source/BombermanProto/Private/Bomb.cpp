// Fill out your copyright notice in the Description page of Project Settings.


#include "Bomb.h"
#include <Components/SkeletalMeshComponent.h>
#include <../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraComponent.h>
#include "BombBlast.h"
#include "Net/UnrealNetwork.h"
#include <Kismet/GameplayStatics.h>
#include <Components/BoxComponent.h>
#include <Kismet/KismetMathLibrary.h>
#include "BombBlast.h"
#include "NBox.h"
#include "NoBox.h"
#include "MapOut.h"
#include <Sound/SoundCue.h>
#include "BBMCharacter.h"
#include "DrawDebugHelpers.h"
#include <Components/CapsuleComponent.h>
#include <CollisionQueryParams.h>
#include "Meteo.h"



// Sets default values
ABomb::ABomb()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//박스 콜리전
	boxcomp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxcomp"));
	RootComponent = boxcomp;

	boxcomp->SetRelativeScale3D(FVector(1.0f));
	boxcomp->SetCollisionProfileName(TEXT("bomb"));
	boxcomp->SetBoxExtent(FVector(90));


	//폭탄 SkeletalMesh 설정
	Bombmesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Bombmesh"));
	Bombmesh->SetRelativeScale3D(FVector(3.0f));
	Bombmesh->SetRelativeLocation(FVector(0, 0, -45));
	Bombmesh->SetupAttachment(RootComponent);
	Bombmesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	USkeletalMesh* tempMesh = LoadObject<USkeletalMesh>(nullptr, TEXT("/Script/Engine.SkeletalMesh'/Game/BBM/Regular_Bomb/Regular_Bomb__Low_Poly_.Regular_Bomb__Low_Poly_'"));

	if (tempMesh != nullptr)
	{
		Bombmesh->SetSkeletalMesh(tempMesh);
	}

	//폭탄 퓨즈 나이아가라 설정
	Fuse_Particle = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Fuse_Particle"));

	ConstructorHelpers::FObjectFinder<UNiagaraSystem>tempNiagara(TEXT("/Script/Niagara.NiagaraSystem'/Game/BomberGuys/Assets/Niagara_Particles/Fuse/Fuse_Particle.Fuse_Particle'"));

	if (tempNiagara.Succeeded())
	{
		UNiagaraSystem* NiagaraSystem = tempNiagara.Object;

		// NiagaraSystem을 Fuse_Particle 컴포넌트에 할당
		Fuse_Particle->SetAsset(NiagaraSystem);

		// 컴포넌트를 월드에 추가
		Fuse_Particle->AttachToComponent(Bombmesh, FAttachmentTransformRules::KeepRelativeTransform);
		Fuse_Particle->SetRelativeLocation(FVector(0, 0, 35));

	}

	//Make Array 부분

	MakeArr[0] = FVector(1, 0, 0);
	MakeArr[1] = FVector(0, 1, 0);
	MakeArr[2] = FVector(-1, 0, 0);
	MakeArr[3] = FVector(0, -1, 0);

	BombSound = CreateDefaultSubobject<USoundWave>(TEXT("BombSound"));

	ConstructorHelpers::FObjectFinder<USoundWave> ExplosionSoundAsset(TEXT("/Script/Engine.SoundWave'/Game/SOUND/bomb.bomb'"));
	if (ExplosionSoundAsset.Succeeded())
	{
		BombSound = ExplosionSoundAsset.Object;
	}

	BlockB = CreateDefaultSubobject<UBoxComponent>(TEXT("BlockB"));
	BlockB->SetupAttachment(RootComponent);
	BlockB->SetCollisionProfileName(TEXT("B_BlockC"));
	BlockB->SetBoxExtent(FVector(51));


	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->ProjectileGravityScale = 1.0f; // 포물선 운동에서 중력을 적용

	Bombmesh->SetEnableGravity(true); // 액터에 중력 활성화
	bReplicates = true; //복제기능을 켠다.
	SetReplicateMovement(true);
}

// Called when the game starts or when spawned
void ABomb::BeginPlay()
{
	Super::BeginPlay();

	boxcomp->OnComponentHit.AddDynamic(this, &ABomb::OnHit);
	boxcomp->OnComponentEndOverlap.AddDynamic(this, &ABomb::OnBombEndOverlap);
	startlocation = boxcomp->GetComponentLocation();

}


// Called every frame
void ABomb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority() == false)
	{
		return;
	}
	//DrawDebugSphere(GetWorld(), startlocation, 160, 30, FColor::Red, false, 10, 1, 2);
	// 현재 시간 흐르게 하기
	currentTime += GetWorld()->DeltaTimeSeconds;

	if (0.28f <= currentTime && currentTime <= 0.7f)
	{
		// 폭탄 스케일 커짐.
		Bombmesh->SetRelativeScale3D(FVector(3.4f));
	}
	if (0.91f <= currentTime && currentTime <= 1.33f)
	{
		// 폭탄 스케일 작아짐.
		Bombmesh->SetRelativeScale3D(FVector(2.0f));
	}
	if (1.33f < currentTime && currentTime <= 1.75f)
	{
		// 폭탄 스케일 커짐.
		Bombmesh->SetRelativeScale3D(FVector(3.4f));
	}
	if (1.96f <= currentTime && currentTime <= 2.38f)
	{
		// 폭탄 스케일 작아짐.
		Bombmesh->SetRelativeScale3D(FVector(2.0f));
	}
	if (2.38f < currentTime && currentTime <= 2.8f)
	{
		// 폭탄 스케일 커짐.
		Bombmesh->SetRelativeScale3D(FVector(3.4f));
	}

	if (!isBombDie)
	{
		if (currentTime >= 3.0f)
		{
			// 폭탄 터짐.
			Explode();
			Destroy();
			if (BombSound)
			{
				UGameplayStatics::PlaySoundAtLocation(this, BombSound, GetActorLocation());
			}
		}
	}
	//blast와 만났으면 시간이 흐른다.
	if (IsBlast)
	{
		//1초가 지나면 폭탄이 폭발한다.
		ExplodeCurrentTime += DeltaTime;
		if (ExplodeCurrentTime > ExplodeCreateTime)
		{
			Explode();
			Destroy();
			if (BombSound)
			{
				UGameplayStatics::PlaySoundAtLocation(this, BombSound, GetActorLocation());
			}
		}
	}
	// 만약 플레이어가 c 버튼을 눌렀다면 
	if (bPlayBombKick)
	{

		FVector StartLocation = GetActorLocation(); // 현재 액터의 위치를 가져옵니다.
		FVector ForwardVector = GetActorForwardVector(); // 현재 액터의 앞 방향 벡터를 가져옵니다.
		float TraceLength = 100.0f; // 라인 트레이스의 길이를 설정합니다.

		FHitResult HitResult;


		// 라인 트레이스를 수행하여 앞 방향에 충돌을 감지합니다.
		bool bHit = GetWorld()->LineTraceSingleByChannel(

			HitResult,
			StartLocation,
			StartLocation + ForwardVector * TraceLength,
			ECC_Visibility

		);

		if (bHit)
		{
			BombKick(DeltaTime);

			AActor* HitActor = HitResult.GetActor();
			UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *HitActor->GetName());

			auto player = Cast<ABBMCharacter>(HitActor);

			if (HitActor != this, HitActor != player)
			{
				UE_LOG(LogTemp, Warning, TEXT("sssssssssssssssss"));
				// isBombSpeed = 0;
				isBombDie = false;
			}
			else
			{

				BombKick(DeltaTime);
			}
		}

	}

}


//폭발 함수
void ABomb::Explode()
{
	auto player = Cast<ABBMCharacter>(GetOwner());
	auto blastowner = Cast<ABombBlast>(GetOwner());
	FVector start = startlocation;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < player->length + 1; j++)
		{
			FVector end = start + MakeArr[i] * 100 * j;
			FHitResult hitresult;
			FCollisionQueryParams CollisionParams;
			CollisionParams.AddIgnoredActor(this);
			bool bhit = GetWorld()->LineTraceSingleByChannel(hitresult, start, end, ECC_Visibility, CollisionParams);

			ABombBlast* blastInstance = GetWorld()->SpawnActor<ABombBlast>(blast, startlocation + MakeArr[i] * 100 * j, UKismetMathLibrary::MakeRotFromX(MakeArr[i]));
			ABBMCharacter* owner = Cast<ABBMCharacter>(GetOwner());
			blastInstance->SetOwner(owner);
			PlayBomb();
			if (bhit)
			{
				UE_LOG(LogTemp, Warning, TEXT("Hit Actor : %s"), *hitresult.GetActor()->GetName());
				// 철박스에 부딪히면 겹치지말고 바로 없어지도록
				if (hitresult.GetActor()->GetName().Contains("NoBox"))
				{
					blastInstance->Destroy();
					UE_LOG(LogTemp, Warning, TEXT("hit ironbox"));
				}
				// 테두리에 부딪혀도 겹치지말고 바로 없어지도록
				else if (hitresult.GetActor()->GetName().Contains("MapOut"))
				{
					blastInstance->Destroy();
					UE_LOG(LogTemp, Warning, TEXT("hit mapout"));
				}
				// 폭탄과 부딪히면 연쇄폭발
				else if (hitresult.GetActor()->IsA(ABomb::StaticClass()))
				{
					ABomb* hitbomb = Cast<ABomb>(hitresult.GetActor());
					hitbomb->IsBlast = true;
					UE_LOG(LogTemp, Warning, TEXT("hit bomb"));
					//철, 테두리는 다시 막히도록
					if (hitresult.GetActor()->GetName().Contains("NoBox") && hitresult.GetActor()->GetName().Contains("MapOut"))
					{
						blastInstance->Destroy();
						UE_LOG(LogTemp, Warning, TEXT("HHHHHHH"));
					}
				}
				// 박스와 부딪히면 박스를 부수고 아이템을 스폰한다
				else if (hitresult.GetActor()->IsA(ANBox::StaticClass()))
				{
// 					ANBox* nbox = Cast<ANBox>(hitresult.GetActor());
// 					RandomSpawn = FMath::RandRange(1, 100);
// 					Randomitem = FMath::RandRange(1, 100);
// 					// 아이템 나올 확률 35퍼
// 					if (RandomSpawn < 36)
// 					{
// 						//아이템을 스폰하겠다.
// 						nbox->SpawnItem(Randomitem, nbox->GetActorLocation());
// 						UE_LOG(LogTemp, Warning, TEXT("Spawn!!!!"));
// 					}
// 					hitresult.GetActor()->Destroy();
// 					//확률값 재설정
// 					RandomSpawn = FMath::RandRange(1, 100);
// 					Randomitem = FMath::RandRange(1, 100);
// 
// 					//UE_LOG(LogTemp, Warning, TEXT("Item Spawn!"));
					break;
				}
				else if (hitresult.GetActor()->IsA(AMeteo::StaticClass()))
				{
// 					/*UE_LOG(LogTemp, Warning, TEXT("hit box"));
// 						break;*/
// 					AMeteo* Meteo = Cast<AMeteo>(hitresult.GetActor());
// 					RandomSpawn2 = FMath::RandRange(1, 100);
// 					Randomitem2 = FMath::RandRange(1, 100);
// 					// 아이템 나올 확률 35퍼
// 					if (RandomSpawn2 < 100)
// 					{
// 						//아이템을 스폰하겠다.
// 						Meteo->SpawnItem(Randomitem2, Meteo->GetActorLocation());
// 						UE_LOG(LogTemp, Warning, TEXT("Spawn!!!!"));
// 					}
// 					hitresult.GetActor()->Destroy();
// 					//확률값 재설정
// 					RandomSpawn2 = FMath::RandRange(1, 100);
// 					Randomitem2 = FMath::RandRange(1, 100);
// 
 					break;
				}
				else if (hitresult.GetActor()->GetName().Contains("Up"))
				{
					
				}
				else
				{
					break;
				}
			}
		}
		for (int n = 1; n < player->length + 1; n++)
		{
			FVector end = start + MakeArr[i] * 100 * n;

			FHitResult hitresult;
			FCollisionQueryParams CollisionParams;
			CollisionParams.AddIgnoredActor(this);

			bool bhit = GetWorld()->LineTraceSingleByChannel(hitresult, start, end, ECC_Visibility, CollisionParams);

			if (bhit)
			{
				if (HasAuthority())
				{
					//태정이형 여길봐20230823
					//맞은 대상이 player라면
					if (hitresult.GetActor()->GetName().Contains("BBMCharacter"))
					{
						ABBMCharacter* hitplayer = Cast<ABBMCharacter>(hitresult.GetActor());
						hitplayer->HitBomb();
					}
					//대상이 철이라면
					else if (hitresult.GetActor()->GetName().Contains("NoBox"))
					{
						//ANoBox* hitIron = Cast<ANoBox>(hitresult2.GetActor());
						//나니모 와캇다..
						UE_LOG(LogTemp, Warning, TEXT("hit iron"));

					}
					//대상이 테두리라면
					else if (hitresult.GetActor()->GetName().Contains("MapOut"))
					{
						//나니모 와캇다..2
						UE_LOG(LogTemp, Warning, TEXT("hit MapOut"));

					}
					//대상이 폭탄이라면
					else if (hitresult.GetActor()->IsA(ABomb::StaticClass()))
					{
						//UE_LOG(LogTemp, Warning, TEXT("hit Bomb"));
						//IsBlast = true;
					}
					else if (hitresult.GetActor()->GetName().Contains("Up"))
					{
						hitresult.GetActor()->Destroy();
						UE_LOG(LogTemp, Warning, TEXT("Item Hit"));

					}
					//대상이 박스라면
					else if (hitresult.GetActor()->IsA(ANBox::StaticClass()))
					{
						/*UE_LOG(LogTemp, Warning, TEXT("hit box"));
						break;*/
						ANBox* nbox = Cast<ANBox>(hitresult.GetActor());

						// 아이템 나올 확률 35퍼
						if (RandomSpawn < 36)
						{
							//아이템을 스폰하겠다.
							nbox->SpawnItem(Randomitem, nbox->GetActorLocation());
							UE_LOG(LogTemp, Warning, TEXT("Spawn!!!!"));
						}
						hitresult.GetActor()->Destroy();
						//확률값 재설정
						RandomSpawn = FMath::RandRange(1, 100);
						Randomitem = FMath::RandRange(1, 100);

						break;
					}
					else if (hitresult.GetActor()->IsA(AMeteo::StaticClass()))
					{
						/*UE_LOG(LogTemp, Warning, TEXT("hit box"));
												break;*/
						AMeteo* Meteo = Cast<AMeteo>(hitresult.GetActor());
						RandomSpawn2 = FMath::RandRange(1, 100);
						Randomitem2 = FMath::RandRange(1, 100);
						// 아이템 나올 확률 35퍼
						if (RandomSpawn2 < 36)
						{
							//아이템을 스폰하겠다.
							Meteo->SpawnItem(Randomitem2, Meteo->GetActorLocation());
							UE_LOG(LogTemp, Warning, TEXT("Spawn!!!!"));
						}
						hitresult.GetActor()->Destroy();
						//확률값 재설정
						RandomSpawn2 = FMath::RandRange(1, 100);
						Randomitem2 = FMath::RandRange(1, 100);

						break;
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("other"));
						hitresult.GetActor()->Destroy();

					}
				}
				break;
			}
		}


	}
}

void ABomb::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ABombBlast* blastBool = Cast<ABombBlast>(OtherActor);
	if (blastBool != nullptr)
	{
		IsBlast = true;
		UE_LOG(LogTemp, Warning, TEXT("IsBlast is true"));
	}
}


void ABomb::BombKick(float deltaTime)
{
	// 폭탄 발로 차기
	p0 = GetActorLocation();
	vt = dir * isBombSpeed * deltaTime;
	p = p0 + vt;
	SetActorLocation(p, true);

	// 폭탄을 발로 차는 중일 때는 폭탄 터지지 않게
	isBombDie = true;

	ABBMCharacter* player = Cast<ABBMCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());

	player->iskick = false;
}


void ABomb::PlayBombKick()
{
	bPlayBombKick = true;
}

void ABomb::getplayer()
{
	ABBMCharacter* player = Cast<ABBMCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), ABBMCharacter::StaticClass()));
	dir = player->GetActorForwardVector();
}

void ABomb::PlayBomb_Implementation()
{
	RandomSpawn = FMath::RandRange(1, 100);
	Randomitem = FMath::RandRange(1, 100);
}

void ABomb::CLearBombCollision()
{
	ABBMCharacter* PlayerCharacter = Cast<ABBMCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (PlayerCharacter)
	{
		UCapsuleComponent* CapsuleComp = PlayerCharacter->GetCapsuleComponent();
		if (CapsuleComp)
		{
			CapsuleComp->ClearMoveIgnoreComponents();
		}
	}
}


void ABomb::OnBombEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	CLearBombCollision();
}


