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
//	//캐릭터 스켈레탈 메시컴포넌트 설정
//	GetMesh()->SetupAttachment(RootComponent);
//	GetMesh()->SetRelativeLocation(FVector(0, 0, -80));
//	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
//	GetMesh()->SetRelativeScale3D(FVector(0.19f));
//
//	//스켈레탈 메시 넣기
//	ConstructorHelpers::FObjectFinder<USkeletalMesh>TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/bbmAssets/Player/bbmOrigin.bbmOrigin'"));
//	if (TempMesh.Succeeded())
//	{
//		GetMesh()->SetSkeletalMesh(TempMesh.Object);
//	}
//
//	//폭탄에 맞았을 때 날아가는 나이아가라
//	blast_Particle = CreateDefaultSubobject<UNiagaraComponent>(TEXT("blast_Particle"));
//
//	ConstructorHelpers::FObjectFinder<UNiagaraSystem>tempNiagara(TEXT("/Script/Niagara.NiagaraSystem'/Game/BomberGuys/Assets/Niagara_Particles/Fireball/Blast_Particle.Blast_Particle'"));
//
//	if (tempNiagara.Succeeded())
//	{
//		UNiagaraSystem* NiagaraSystem = tempNiagara.Object;
//
//		// NiagaraSystem을 Fuse_Particle 컴포넌트에 할당
//		blast_Particle->SetAsset(NiagaraSystem);
//
//		// 컴포넌트를 월드에 추가
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
//	//폭탄에 맞아 날아간다면
//	if (IsFly)
//	{
//		auto anim = Cast<UPlayerAnim>(GetMesh()->GetAnimInstance());
//		if (anim)
//		{
//			anim->PlayFlyAnimation();
//		}
//
//		//날아갈때 생기는 효과를 true로 바꾼다.
//		blast_Particle->SetVisibility(true);
//
//		//펑 터져 날아가는 시간값이 증가한다.
//		FlyCurrentTime += DeltaTime;
//
//		//P=P0+vt로 캐릭터를 날린다.
//		FVector P = GetActorLocation() + FlySpeed * FlyDirection.Normalize() * DeltaTime;
//		SetActorLocation(P);
//		//만약 3초동안 날아갔다면
//		if (FlyCurrentTime > FlyCreateTime)
//		{
//			blast_Particle->SetVisibility(false);
//			//캐릭터가 죽는다.
//			Destroy();
//			//시간을 초기화해준다.
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
//	//만약 폭탄에 맞아 날아가야한다면
//
//	//날아가는 상태를 true로 바꾼다.
//	IsFly = true;
//}

