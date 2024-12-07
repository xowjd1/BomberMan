// Fill out your copyright notice in the Description page of Project Settings.


#include "BBMCharacter.h"
#include "NStartBoxSpawn.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include <Animation/AnimInstance.h>
#include "PlayerAnim.h"
#include <../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraComponent.h>
#include <../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputMappingContext.h>
#include <../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputAction.h>
#include <kismet/GameplayStatics.h>
#include "BombBlast.h"
#include "Net/UnrealNetwork.h"
#include <GameFramework/CharacterMovementComponent.h>
#include "Bomb.h"
#include <Kismet/KismetMathLibrary.h>
#include <Kismet/KismetArrayLibrary.h>
#include "GameFramework/Character.h"
#include <Components/CapsuleComponent.h>
#include <Sound/SoundWave.h>
#include <UMG/Public/Components/WidgetComponent.h>
#include "GameFramework/PlayerState.h"
#include "GameFramework/Actor.h"
#include "PlayerInfoWidget.h"
#include <UMG/Public/Components/TextBlock.h>
#include "WaitingUI.h"
#include <UMG/Public/Components/Image.h>
#include <UMG/Public/Components/Button.h>
#include <Materials/MaterialInterface.h>
#include <Materials/MaterialInstanceDynamic.h>
#include "NetGameInstance.h"
#include "ScoreWidget.h"




// Sets default values
ABBMCharacter::ABBMCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//캐릭터 스켈레탈 메시컴포넌트 설정

	GetMesh()->SetupAttachment(RootComponent);
	GetMesh()->SetRelativeLocation(FVector(0, 0, -80));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	GetMesh()->SetRelativeScale3D(FVector(0.19f));


	// 폭탄 인식 콜리전 설정
	TriggerComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("TriggerComp"));
	TriggerComp->SetCapsuleHalfHeight(417.0f);
	TriggerComp->SetCapsuleRadius(148.0f);
	TriggerComp->SetRelativeLocation(GetActorLocation());
	TriggerComp->SetupAttachment(GetMesh());

	TriggerComp->SetRelativeLocation(FVector(-12.0f, 157.0f, 473.0f));
	TriggerComp->SetRelativeScale3D(FVector(1));

	//스켈레탈 메시 넣기
	ConstructorHelpers::FObjectFinder<USkeletalMesh>TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/bbmAssets/Player/bbmOrigin.bbmOrigin'"));
	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
	}


	//수정 필요
	//코드에서 imc 매핑
	ConstructorHelpers::FObjectFinder<UInputMappingContext>mappingContext(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Bluprints/enhancedInput/IMC_Character.IMC_Character'"));
	if (mappingContext.Succeeded())
	{
		defaultContext = mappingContext.Object;
	}
	//코드에서 IA Movement 매핑
	ConstructorHelpers::FObjectFinder<UInputAction>movementContext(TEXT("/Script/EnhancedInput.InputAction'/Game/Bluprints/enhancedInput/IA_Movement.IA_Movement'"));
	if (movementContext.Succeeded())
	{
		MovementAction = movementContext.Object;
	}
	//코드에서 IA SetBomb 매핑
	ConstructorHelpers::FObjectFinder<UInputAction>setBombContext(TEXT("/Script/EnhancedInput.InputAction'/Game/Bluprints/enhancedInput/IA_SetBomb.IA_SetBomb'"));
	if (setBombContext.Succeeded())
	{
		SetBombAction = setBombContext.Object;
	}
	//코드에서 IA Bombing 매핑
	ConstructorHelpers::FObjectFinder<UInputAction>BombingContext(TEXT("/Script/EnhancedInput.InputAction'/Game/Bluprints/enhancedInput/IA_Bombing.IA_Bombing'"));
	if (BombingContext.Succeeded())
	{
		BombingAction = BombingContext.Object;
	}
	//코드에서 IA Chat 매핑
	ConstructorHelpers::FObjectFinder<UInputAction>ChattingContext(TEXT("/Script/EnhancedInput.InputAction'/Game/Bluprints/enhancedInput/IA_Chat.IA_Chat'"));
	if (ChattingContext.Succeeded())
	{
		ChattingAction = ChattingContext.Object;
	}

	//폭탄에 맞았을 때 날아가는 나이아가라
	blast_Particle = CreateDefaultSubobject<UNiagaraComponent>(TEXT("blast_Particle"));

	ConstructorHelpers::FObjectFinder<UNiagaraSystem>tempNiagara(TEXT("/Script/Niagara.NiagaraSystem'/Game/BomberGuys/Assets/Niagara_Particles/Fireball/Blast_Particle.Blast_Particle'"));

	if (tempNiagara.Succeeded())
	{
		UNiagaraSystem* NiagaraSystem = tempNiagara.Object;

		// NiagaraSystem을 Fuse_Particle 컴포넌트에 할당
		blast_Particle->SetAsset(NiagaraSystem);
	}

	DieSound = CreateDefaultSubobject<USoundWave>(TEXT("DieSound"));

	ConstructorHelpers::FObjectFinder<USoundWave> DieSoundAsset(TEXT("/Script/Engine.SoundWave'/Game/SOUND/die.die'"));
	if (DieSoundAsset.Succeeded())
	{
		DieSound = DieSoundAsset.Object;
	}

	BombPutSound = CreateDefaultSubobject<USoundWave>(TEXT("BombPutSound"));

	ConstructorHelpers::FObjectFinder<USoundWave> BombPutSoundAsset(TEXT("/Script/Engine.SoundWave'/Game/SOUND/bomb-put.bomb-put'"));
	if (BombPutSoundAsset.Succeeded())
	{
		BombPutSound = BombPutSoundAsset.Object;
	}

	//플레이어 닉네임을 띄울 위젯을 메쉬에 붙이겠다.
	infoWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Player Info Widget"));
	infoWidget->SetupAttachment(GetMesh());
	bReplicates = true; //복제기능을 켠다.


}

// Called when the game starts or when spawned
void ABBMCharacter::BeginPlay()
{
	Super::BeginPlay();
	//enhanced input
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController())) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->AddMappingContext(defaultContext, 0);
		}
	}
	//게임시작 나오게
	UWaitingUI* waitWidgetInst = CreateWidget<UWaitingUI>(GetWorld(), waitingui);
	gi = GetGameInstance<UNetGameInstance>();
	if (waitWidgetInst != nullptr)
	{
		if (HasAuthority() && GetController() != nullptr && GetController()->IsLocalPlayerController())
		{
			if (!gi->isclicked)
			{
				waitWidgetInst->AddToViewport();
				waitWidgetInst->img_start->SetVisibility(ESlateVisibility::Visible);
				waitWidgetInst->btn_start->SetVisibility(ESlateVisibility::Visible);
				gi->isclicked = true;
			}

		}

	}

	//초기 불꽃길이 설정
	if (HasAuthority())
	{
		length = initialLength;
		UE_LOG(LogTemp, Warning, TEXT("netmode : %d, length : %f"), GetNetMode(), length);
	}

	myLocalRole = GetLocalRole();
	myRemoteRole = GetRemoteRole();

	if (GetController() != nullptr && GetController()->IsLocalPlayerController())
	{
		gi = GetGameInstance<UNetGameInstance>();
		ServerSetInitinfo(gi->playerInitInfo);
	}


	if (blast_Particle != nullptr)
	{
		// 컴포넌트를 월드에 추가
		blast_Particle->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);
		blast_Particle->RegisterComponent();
		blast_Particle->SetRelativeLocation(FVector(0));
		blast_Particle->SetVisibility(false);
	}


	TriggerComp->OnComponentBeginOverlap.AddDynamic(this, &ABBMCharacter::OnComponentBeginOverlap);
	TriggerComp->OnComponentEndOverlap.AddDynamic(this, &ABBMCharacter::OnTriggerEndOverlap);



	callBombCollision.AddDynamic(this, &ABBMCharacter::ClearBombCollision);

	//닉네임 받아와서 띄우는 부분
	if (infoWidget != nullptr)
	{
		info_UI = Cast<UPlayerInfoWidget>(infoWidget->GetWidget());

	}

	// 캐릭터 초기화 지연실행
	FTimerHandle initHandler;
	GetWorldTimerManager().SetTimer(initHandler, this, &ABBMCharacter::InitializePlayer, 0.5f, false);
}

// Called every frame
void ABBMCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//if (GetController() != nullptr && GetController()->IsLocalPlayerController())
	{
		//UE_LOG(LogTemp, Warning, TEXT("netmode : %d, length : %f"), GetNetMode(), length);
	}

	//폭탄에 맞아 날아간다면
	if (IsFly)
	{
		auto anim = Cast<UPlayerAnim>(GetMesh()->GetAnimInstance());
		if (anim)
		{
			anim->PlayFlyAnimation();
		}

		//날아갈때 생기는 효과를 true로 바꾼다.
		blast_Particle->SetVisibility(true);

		//펑 터져 날아가는 시간값이 증가한다.
		FlyCurrentTime += DeltaTime;

		//P=P0+vt로 캐릭터를 날린다.
		FVector P = GetActorLocation() + FlySpeed * FlyDirection.Normalize() * DeltaTime;
		SetActorLocation(P);
		//만약 3초동안 날아갔다면
		if (FlyCurrentTime > FlyCreateTime)
		{
			isScoreCheck = true;
			blast_Particle->SetVisibility(false);

			//캐릭터가 죽는다.
			Destroy();
			//시간을 초기화해준다.
			FlyCurrentTime = 0;
		}
	}
}

// Called to bind functionality to input
void ABBMCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &ABBMCharacter::Move);
		EnhancedInputComponent->BindAction(SetBombAction, ETriggerEvent::Started, this, &ABBMCharacter::SetBomb);
		EnhancedInputComponent->BindAction(BombingAction, ETriggerEvent::Started, this, &ABBMCharacter::Bombing);
	}

	PlayerInputComponent->BindAction(TEXT("BoxSkip"), IE_Pressed, this, &ABBMCharacter::BoxSkipBtn);
	UE_LOG(LogTemp, Warning, TEXT("BindAction called for BoxSkip"));

	PlayerInputComponent->BindAction(TEXT("BombCatch"), IE_Pressed, this, &ABBMCharacter::BombCatch);

}

void ABBMCharacter::Move(const FInputActionValue& value)
{

	const FVector2D MovementVector = value.Get<FVector2D>();
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(ForwardDirection, MovementVector.Y);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(RightDirection, MovementVector.X);


	//여길봐
	// 방향값으로 캐릭터 회전
	if (!MovementVector.IsNearlyZero())
	{
		const float RotationAngle = FMath::Atan2(-MovementVector.Y, MovementVector.X) * 180.f / PI;
		FRotator CurrentRotation = GetActorRotation();
		CurrentRotation.Yaw = RotationAngle;
		SetActorRotation(CurrentRotation);
	}
}

bool ABBMCharacter::ServerSetBomb_Validate()
{
	return true;
}

void ABBMCharacter::ServerSetBomb_Implementation()
{
	if (BombPutSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, BombPutSound, GetActorLocation());
	}

	FIntPoint BombGridCell = GetGridCell(GetActorLocation());

	if (!IsGridCellOccupied(BombGridCell))
	{
		if ((placedBombs.Num()) < bombCount)
		{
			FActorSpawnParameters param;
			param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			FVector SpawnLocation = FVector(BombGridCell.X * 100.0f, BombGridCell.Y * 100.0f, GetActorLocation().Z + 0.0f); // 위로 50 단위 이동
			ABomb* spawnedBomb = GetWorld()->SpawnActor<ABomb>(bombActor, SpawnLocation, GetActorRotation(), param);
			spawnedBomb->SetOwner(this);
			placedBombs.AddUnique(spawnedBomb);

			UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
			if (CapsuleComp)
			{
				CapsuleComp->IgnoreComponentWhenMoving(spawnedBomb->BlockB, true);

			}
			spawnedBomb->OnDestroyed.AddDynamic(this, &ABBMCharacter::OnBombDestroyed);
		}
	}
}

void ABBMCharacter::SetBomb()
{
	if (HasAuthority())  // 서버인 경우에만 실행
	{
		ServerSetBomb();
	}
	else if (GetLocalRole() == ROLE_AutonomousProxy)  // 클라이언트인 경우에만 실행
	{
		// 클라이언트에서 서버로 리모트 호출하여 폭탄 생성 요청 보내기
		ServerSetBomb();
		UE_LOG(LogTemp, Warning, TEXT("ah ni jom doi la"));
	}
}

void ABBMCharacter::HitBomb()
{

	//만약 폭탄에 맞아 날아가야한다면

	//날아가는 상태를 true로 바꾼다.
	IsFly = true;

	// Die사운드 재생
	if (DieSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DieSound, GetActorLocation());
	}
}


void ABBMCharacter::BombCatch()
{
	// 배열에 액터 담기
	TArray<AActor*> OverlappingActors;
	// 오버랩 된 폭탄들을 배열로 받아온다
	GetOverlappingActors(OverlappingActors, ABomb::StaticClass()); // ABomb 대신 해당되는 액터 클래스를 사용하세요

	// 가장 가까운 폭탄을 찾기 위한 변수
	// 초기값으로 가장 큰 값
	float ClosestDistance = FLT_MAX;
	// 가장 가까운 폭탄을 저장할 변수
	ClosestBomb = nullptr;

	// 배열을 돌면서 가장 가까운 폭탄 찾기
	for (AActor* Actor : OverlappingActors)
	{
		ABomb* BombClass = Cast<ABomb>(Actor);
		// 만약 폭탄이 있다면
		if (BombClass)
		{
			FVector BombLocation = BombClass->GetActorLocation();
			FVector PlayerLocation = GetActorLocation();
			// 폭탄과 나의 거리재기
			float Distance = FVector::Distance(PlayerLocation, BombLocation);

			// 폭탄과 나의 거리가 100 이하라면
			if (Distance <= 100.0f && Distance < ClosestDistance)
			{
				// 가장 가까운 폭탄 찾기
				ClosestDistance = Distance;
				ClosestBomb = BombClass;
			}
		}
	}
	// 만약 가장 가까운 폭탄이 있다면
	if (ClosestBomb)
	{
		FVector PlayerLocation = GetActorLocation();
		// 플레이어의 자식으로 넣어주기
		ClosestBomb->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
		// 플레이어의 머리 위로 폭탄을 배치하기
		FVector NewBombLocation = FVector(PlayerLocation.X, PlayerLocation.Y, PlayerLocation.Z + 145.0f);
		ClosestBomb->SetActorLocation(NewBombLocation);
		// 폭탄을 잡았을 때는 폭탄 터지지 않게
		ClosestBomb->isBombDie = true;
	}

	if (!ClosestBomb)
	{
		ClosestBomb->isBombDie = false;
	}
}

void ABBMCharacter::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{


	UE_LOG(LogTemp, Warning, TEXT("OnComponentBeginOverlap : %s"), *OtherActor->GetName());

	// 오버랩 중인 액터를 변수에 저장
	OverlappingActor = OtherActor;
	//ClosestBomb->isTrigger = true;
	ABomb* abomb = Cast<ABomb>(OtherActor);

	if (abomb != nullptr)
	{
		iskick = true;
		UE_LOG(LogTemp, Warning, TEXT("isKick is true"));
	}


}


// 아이템 봄킥 함수
void ABBMCharacter::Bombing()
{
	if (iskick)
	{
		UE_LOG(LogTemp, Warning, TEXT("Bombing"));
		// 발로 차는 중인지 감지
		//isBombing = true;
		//ClosestBomb->isTrigger = true;
		// 오버랩 액터를 ABomb 으로 캐스팅 했어.
		auto bomb = Cast<ABomb>(OverlappingActor);
		// 캐스팅이 성공했다면
		if (bomb)
		{
			UE_LOG(LogTemp, Warning, TEXT("2222222222222222222"));
			// -> 폭탄의 BombKick 을 플레이 하도록 하고 싶다.
			bomb->PlayBombKick();
			bomb->isBombDie = false;
			bomb->getplayer();
		}
	}
}

void ABBMCharacter::OnTriggerEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ABomb* abomb = Cast<ABomb>(OtherActor);

	if (abomb != nullptr)
	{
		if (iskick)
		{
			UE_LOG(LogTemp, Warning, TEXT("no"));
			abomb->isBombSpeed = 0;
			iskick = false;
		}
	}
}

void ABBMCharacter::BoxSkipBtn()
{
	ANStartBoxSpawn* NStartBoxSpawn = Cast<ANStartBoxSpawn>(UGameplayStatics::GetActorOfClass(GetWorld(), ANStartBoxSpawn::StaticClass()));

	NStartBoxSpawn->TimerCurrentTime = 60;
}


void ABBMCharacter::SpeedUp()
{
	// Value 마지막 숫자값이 아이템이 증가시켜주는 속도값
	float Value = GetCharacterMovement()->MaxWalkSpeed + 200;
	float MinValue = 0.0f;
	float MaxValue = 1200.0f; //최대 증가수치값

	float ClampValue = FMath::Clamp(Value, MinValue, MaxValue);

	GetCharacterMovement()->MaxWalkSpeed = ClampValue;
}

void ABBMCharacter::UpgradeBombCount()
{
	// Value 마지막 숫자값이 아이템을 먹었을 때 폭탄이 몇개 증가하는지
	float Value = bombCount + 1;
	float MinValue = 0.0f;
	float MaxValue = 4.0f; //최대 증가수치값

	float ClampValue = FMath::Clamp(Value, MinValue, MaxValue);

	bombCount = ClampValue;
	//UE_LOG(LogTemp,Warning,TEXT("upgradebombcount"));
}


void ABBMCharacter::ClearBombCollision()
{
	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	CapsuleComp->ClearMoveIgnoreComponents();
}

void ABBMCharacter::OnBombDestroyed(AActor* DestroyedActor)
{
	// 파괴된 폭탄을 placedBombs 배열에서 제거
	placedBombs.Remove(DestroyedActor);
}

bool ABBMCharacter::IsGridCellOccupied(const FIntPoint& GridCell) const
{
	for (AActor* PlacedBomb : placedBombs)
	{
		if (PlacedBomb)
		{
			const FVector& BombLocation = PlacedBomb->GetActorLocation();
			FIntPoint BombGridCell = GetGridCell(BombLocation);
			if (BombGridCell == GridCell)
			{
				return true;
			}
		}
	}
	return false;
}


FIntPoint ABBMCharacter::GetGridCell(const FVector& WorldLocation) const
{
	FVector SnappedLocation = UKismetMathLibrary::Vector_SnappedToGrid(WorldLocation, 100);
	int32 GridX = FMath::RoundToInt(SnappedLocation.X / 100.0f);
	int32 GridY = FMath::RoundToInt(SnappedLocation.Y / 100.0f);
	return FIntPoint(GridX, GridY);
}

//이름 설정 RPC 함수
void ABBMCharacter::ServerSetInitinfo_Implementation(FPlayerInitInfo InitInfo)
{
	myName = InitInfo.userName;
	myMeshColor = InitInfo.meshColor;
}

// 플레이어 초기화 함수
void ABBMCharacter::InitializePlayer()
{
	if (info_UI != nullptr)
	{
		info_UI->text_userName->SetText(FText::FromString(myName));
	}
	//컬러 설정
	UMaterialInterface* mat1 = GetMesh()->GetMaterial(0);

	UMaterialInstanceDynamic* dynamicMat1 = UMaterialInstanceDynamic::Create(mat1, this);

	GetMesh()->SetMaterial(0, dynamicMat1);

	dynamicMat1->SetVectorParameterValue(FName("MyColor"), myMeshColor);
}

void ABBMCharacter::PowerUp_Implementation()
{
	//ABombBlast::length += 100;
	UE_LOG(LogTemp, Warning, TEXT("PowerUp"));
	length += 1;
}

void ABBMCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABBMCharacter, myName);
	DOREPLIFETIME(ABBMCharacter, length);
	DOREPLIFETIME(ABBMCharacter, myMeshColor);

}

