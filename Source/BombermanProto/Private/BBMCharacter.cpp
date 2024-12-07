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

	//ĳ���� ���̷�Ż �޽�������Ʈ ����

	GetMesh()->SetupAttachment(RootComponent);
	GetMesh()->SetRelativeLocation(FVector(0, 0, -80));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	GetMesh()->SetRelativeScale3D(FVector(0.19f));


	// ��ź �ν� �ݸ��� ����
	TriggerComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("TriggerComp"));
	TriggerComp->SetCapsuleHalfHeight(417.0f);
	TriggerComp->SetCapsuleRadius(148.0f);
	TriggerComp->SetRelativeLocation(GetActorLocation());
	TriggerComp->SetupAttachment(GetMesh());

	TriggerComp->SetRelativeLocation(FVector(-12.0f, 157.0f, 473.0f));
	TriggerComp->SetRelativeScale3D(FVector(1));

	//���̷�Ż �޽� �ֱ�
	ConstructorHelpers::FObjectFinder<USkeletalMesh>TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/bbmAssets/Player/bbmOrigin.bbmOrigin'"));
	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
	}


	//���� �ʿ�
	//�ڵ忡�� imc ����
	ConstructorHelpers::FObjectFinder<UInputMappingContext>mappingContext(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Bluprints/enhancedInput/IMC_Character.IMC_Character'"));
	if (mappingContext.Succeeded())
	{
		defaultContext = mappingContext.Object;
	}
	//�ڵ忡�� IA Movement ����
	ConstructorHelpers::FObjectFinder<UInputAction>movementContext(TEXT("/Script/EnhancedInput.InputAction'/Game/Bluprints/enhancedInput/IA_Movement.IA_Movement'"));
	if (movementContext.Succeeded())
	{
		MovementAction = movementContext.Object;
	}
	//�ڵ忡�� IA SetBomb ����
	ConstructorHelpers::FObjectFinder<UInputAction>setBombContext(TEXT("/Script/EnhancedInput.InputAction'/Game/Bluprints/enhancedInput/IA_SetBomb.IA_SetBomb'"));
	if (setBombContext.Succeeded())
	{
		SetBombAction = setBombContext.Object;
	}
	//�ڵ忡�� IA Bombing ����
	ConstructorHelpers::FObjectFinder<UInputAction>BombingContext(TEXT("/Script/EnhancedInput.InputAction'/Game/Bluprints/enhancedInput/IA_Bombing.IA_Bombing'"));
	if (BombingContext.Succeeded())
	{
		BombingAction = BombingContext.Object;
	}
	//�ڵ忡�� IA Chat ����
	ConstructorHelpers::FObjectFinder<UInputAction>ChattingContext(TEXT("/Script/EnhancedInput.InputAction'/Game/Bluprints/enhancedInput/IA_Chat.IA_Chat'"));
	if (ChattingContext.Succeeded())
	{
		ChattingAction = ChattingContext.Object;
	}

	//��ź�� �¾��� �� ���ư��� ���̾ư���
	blast_Particle = CreateDefaultSubobject<UNiagaraComponent>(TEXT("blast_Particle"));

	ConstructorHelpers::FObjectFinder<UNiagaraSystem>tempNiagara(TEXT("/Script/Niagara.NiagaraSystem'/Game/BomberGuys/Assets/Niagara_Particles/Fireball/Blast_Particle.Blast_Particle'"));

	if (tempNiagara.Succeeded())
	{
		UNiagaraSystem* NiagaraSystem = tempNiagara.Object;

		// NiagaraSystem�� Fuse_Particle ������Ʈ�� �Ҵ�
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

	//�÷��̾� �г����� ��� ������ �޽��� ���̰ڴ�.
	infoWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Player Info Widget"));
	infoWidget->SetupAttachment(GetMesh());
	bReplicates = true; //��������� �Ҵ�.


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
	//���ӽ��� ������
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

	//�ʱ� �Ҳɱ��� ����
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
		// ������Ʈ�� ���忡 �߰�
		blast_Particle->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);
		blast_Particle->RegisterComponent();
		blast_Particle->SetRelativeLocation(FVector(0));
		blast_Particle->SetVisibility(false);
	}


	TriggerComp->OnComponentBeginOverlap.AddDynamic(this, &ABBMCharacter::OnComponentBeginOverlap);
	TriggerComp->OnComponentEndOverlap.AddDynamic(this, &ABBMCharacter::OnTriggerEndOverlap);



	callBombCollision.AddDynamic(this, &ABBMCharacter::ClearBombCollision);

	//�г��� �޾ƿͼ� ���� �κ�
	if (infoWidget != nullptr)
	{
		info_UI = Cast<UPlayerInfoWidget>(infoWidget->GetWidget());

	}

	// ĳ���� �ʱ�ȭ ��������
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

	//��ź�� �¾� ���ư��ٸ�
	if (IsFly)
	{
		auto anim = Cast<UPlayerAnim>(GetMesh()->GetAnimInstance());
		if (anim)
		{
			anim->PlayFlyAnimation();
		}

		//���ư��� ����� ȿ���� true�� �ٲ۴�.
		blast_Particle->SetVisibility(true);

		//�� ���� ���ư��� �ð����� �����Ѵ�.
		FlyCurrentTime += DeltaTime;

		//P=P0+vt�� ĳ���͸� ������.
		FVector P = GetActorLocation() + FlySpeed * FlyDirection.Normalize() * DeltaTime;
		SetActorLocation(P);
		//���� 3�ʵ��� ���ư��ٸ�
		if (FlyCurrentTime > FlyCreateTime)
		{
			isScoreCheck = true;
			blast_Particle->SetVisibility(false);

			//ĳ���Ͱ� �״´�.
			Destroy();
			//�ð��� �ʱ�ȭ���ش�.
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


	//�����
	// ���Ⱚ���� ĳ���� ȸ��
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
			FVector SpawnLocation = FVector(BombGridCell.X * 100.0f, BombGridCell.Y * 100.0f, GetActorLocation().Z + 0.0f); // ���� 50 ���� �̵�
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
	if (HasAuthority())  // ������ ��쿡�� ����
	{
		ServerSetBomb();
	}
	else if (GetLocalRole() == ROLE_AutonomousProxy)  // Ŭ���̾�Ʈ�� ��쿡�� ����
	{
		// Ŭ���̾�Ʈ���� ������ ����Ʈ ȣ���Ͽ� ��ź ���� ��û ������
		ServerSetBomb();
		UE_LOG(LogTemp, Warning, TEXT("ah ni jom doi la"));
	}
}

void ABBMCharacter::HitBomb()
{

	//���� ��ź�� �¾� ���ư����Ѵٸ�

	//���ư��� ���¸� true�� �ٲ۴�.
	IsFly = true;

	// Die���� ���
	if (DieSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DieSound, GetActorLocation());
	}
}


void ABBMCharacter::BombCatch()
{
	// �迭�� ���� ���
	TArray<AActor*> OverlappingActors;
	// ������ �� ��ź���� �迭�� �޾ƿ´�
	GetOverlappingActors(OverlappingActors, ABomb::StaticClass()); // ABomb ��� �ش�Ǵ� ���� Ŭ������ ����ϼ���

	// ���� ����� ��ź�� ã�� ���� ����
	// �ʱⰪ���� ���� ū ��
	float ClosestDistance = FLT_MAX;
	// ���� ����� ��ź�� ������ ����
	ClosestBomb = nullptr;

	// �迭�� ���鼭 ���� ����� ��ź ã��
	for (AActor* Actor : OverlappingActors)
	{
		ABomb* BombClass = Cast<ABomb>(Actor);
		// ���� ��ź�� �ִٸ�
		if (BombClass)
		{
			FVector BombLocation = BombClass->GetActorLocation();
			FVector PlayerLocation = GetActorLocation();
			// ��ź�� ���� �Ÿ����
			float Distance = FVector::Distance(PlayerLocation, BombLocation);

			// ��ź�� ���� �Ÿ��� 100 ���϶��
			if (Distance <= 100.0f && Distance < ClosestDistance)
			{
				// ���� ����� ��ź ã��
				ClosestDistance = Distance;
				ClosestBomb = BombClass;
			}
		}
	}
	// ���� ���� ����� ��ź�� �ִٸ�
	if (ClosestBomb)
	{
		FVector PlayerLocation = GetActorLocation();
		// �÷��̾��� �ڽ����� �־��ֱ�
		ClosestBomb->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
		// �÷��̾��� �Ӹ� ���� ��ź�� ��ġ�ϱ�
		FVector NewBombLocation = FVector(PlayerLocation.X, PlayerLocation.Y, PlayerLocation.Z + 145.0f);
		ClosestBomb->SetActorLocation(NewBombLocation);
		// ��ź�� ����� ���� ��ź ������ �ʰ�
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

	// ������ ���� ���͸� ������ ����
	OverlappingActor = OtherActor;
	//ClosestBomb->isTrigger = true;
	ABomb* abomb = Cast<ABomb>(OtherActor);

	if (abomb != nullptr)
	{
		iskick = true;
		UE_LOG(LogTemp, Warning, TEXT("isKick is true"));
	}


}


// ������ ��ű �Լ�
void ABBMCharacter::Bombing()
{
	if (iskick)
	{
		UE_LOG(LogTemp, Warning, TEXT("Bombing"));
		// �߷� ���� ������ ����
		//isBombing = true;
		//ClosestBomb->isTrigger = true;
		// ������ ���͸� ABomb ���� ĳ���� �߾�.
		auto bomb = Cast<ABomb>(OverlappingActor);
		// ĳ������ �����ߴٸ�
		if (bomb)
		{
			UE_LOG(LogTemp, Warning, TEXT("2222222222222222222"));
			// -> ��ź�� BombKick �� �÷��� �ϵ��� �ϰ� �ʹ�.
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
	// Value ������ ���ڰ��� �������� ���������ִ� �ӵ���
	float Value = GetCharacterMovement()->MaxWalkSpeed + 200;
	float MinValue = 0.0f;
	float MaxValue = 1200.0f; //�ִ� ������ġ��

	float ClampValue = FMath::Clamp(Value, MinValue, MaxValue);

	GetCharacterMovement()->MaxWalkSpeed = ClampValue;
}

void ABBMCharacter::UpgradeBombCount()
{
	// Value ������ ���ڰ��� �������� �Ծ��� �� ��ź�� � �����ϴ���
	float Value = bombCount + 1;
	float MinValue = 0.0f;
	float MaxValue = 4.0f; //�ִ� ������ġ��

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
	// �ı��� ��ź�� placedBombs �迭���� ����
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

//�̸� ���� RPC �Լ�
void ABBMCharacter::ServerSetInitinfo_Implementation(FPlayerInitInfo InitInfo)
{
	myName = InitInfo.userName;
	myMeshColor = InitInfo.meshColor;
}

// �÷��̾� �ʱ�ȭ �Լ�
void ABBMCharacter::InitializePlayer()
{
	if (info_UI != nullptr)
	{
		info_UI->text_userName->SetText(FText::FromString(myName));
	}
	//�÷� ����
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

