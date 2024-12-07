// Fill out your copyright notice in the Description page of Project Settings.


#include "BBM2PCharacter.h"
//#include "EnhancedInputComponent.h"
//#include "EnhancedInputSubsystems.h"
//#include <Animation/AnimInstance.h>
//#include "PlayerAnim.h"
//#include <../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraComponent.h>
//
//// Sets default values
//ABBM2PCharacter::ABBM2PCharacter()
//{
// 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
//	PrimaryActorTick.bCanEverTick = true;
//
//	//ĳ���� ���̷�Ż �޽�������Ʈ ����
//	GetMesh()->SetupAttachment(RootComponent);
//	GetMesh()->SetRelativeLocation(FVector(0, 0, -80));
//	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
//	GetMesh()->SetRelativeScale3D(FVector(0.19f));
//
//	//���̷�Ż �޽� �ֱ�
//	ConstructorHelpers::FObjectFinder<USkeletalMesh>TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/bbmAssets/Player/bbmOrigin.bbmOrigin'"));
//	if (TempMesh.Succeeded())
//	{
//		GetMesh()->SetSkeletalMesh(TempMesh.Object);
//	}
//
//	//��ź�� �¾��� �� ���ư��� ���̾ư���
//	blast_Particle = CreateDefaultSubobject<UNiagaraComponent>(TEXT("blast_Particle"));
//
//	ConstructorHelpers::FObjectFinder<UNiagaraSystem>tempNiagara(TEXT("/Script/Niagara.NiagaraSystem'/Game/BomberGuys/Assets/Niagara_Particles/Fireball/Blast_Particle.Blast_Particle'"));
//
//	if (tempNiagara.Succeeded())
//	{
//		UNiagaraSystem* NiagaraSystem = tempNiagara.Object;
//
//		// NiagaraSystem�� Fuse_Particle ������Ʈ�� �Ҵ�
//		blast_Particle->SetAsset(NiagaraSystem);
//
//		// ������Ʈ�� ���忡 �߰�
//		//blast_Particle->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);
//		//blast_Particle->RegisterComponent();
//		//blast_Particle->SetRelativeLocation(FVector(0));
//		//blast_Particle->SetVisibility(false);
//	}
//}
//
//// Called when the game starts or when spawned
//void ABBM2PCharacter::BeginPlay()
//{
//	Super::BeginPlay();
//	
//	//enhanced input
//	if (APlayerController* PlayerController = Cast<APlayerController>(GetController())) {
//		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
//			Subsystem->AddMappingContext(defaultContext, 0);
//		}
//	}
//}
//
//// Called every frame
//void ABBM2PCharacter::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//	//��ź�� �¾� ���ư��ٸ�
//	if (IsFly)
//	{
//		auto anim = Cast<UPlayerAnim>(GetMesh()->GetAnimInstance());
//		if (anim)
//		{
//			anim->PlayFlyAnimation();
//		}
//
//		//���ư��� ����� ȿ���� true�� �ٲ۴�.
//		blast_Particle->SetVisibility(true);
//
//		//�� ���� ���ư��� �ð����� �����Ѵ�.
//		FlyCurrentTime += DeltaTime;
//
//		//P=P0+vt�� ĳ���͸� ������.
//		FVector P = GetActorLocation() + FlySpeed * FlyDirection.Normalize() * DeltaTime;
//		SetActorLocation(P);
//		//���� 3�ʵ��� ���ư��ٸ�
//		if (FlyCurrentTime > FlyCreateTime)
//		{
//			blast_Particle->SetVisibility(false);
//			//ĳ���Ͱ� �״´�.
//			Destroy();
//			//�ð��� �ʱ�ȭ���ش�.
//			FlyCurrentTime = 0;
//		}
//	}
//}
//
//// Called to bind functionality to input
//void ABBM2PCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
//{
//	Super::SetupPlayerInputComponent(PlayerInputComponent);
//	
//	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
//		EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &ABBM2PCharacter::Move);
//		EnhancedInputComponent->BindAction(SetBombAction, ETriggerEvent::Triggered, this, &ABBM2PCharacter::SetBomb);
//	}
//}
//
//void ABBM2PCharacter::Move(const FInputActionValue& value)
//{
//	const FVector2D MovementVector = value.Get<FVector2D>();
//	const FRotator Rotation = Controller->GetControlRotation();
//	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
//	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
//	AddMovementInput(ForwardDirection, MovementVector.Y);
//	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
//	AddMovementInput(RightDirection, MovementVector.X);
//}
//
//void ABBM2PCharacter::SetBomb()
//{
//
//}
//
//void ABBM2PCharacter::HitBomb()
//{
//	//���� ��ź�� �¾� ���ư����Ѵٸ�
//
//	//���ư��� ���¸� true�� �ٲ۴�.
//	IsFly = true;
//}

