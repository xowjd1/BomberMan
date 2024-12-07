// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Bomb.h"
#include "BBMCharacter.generated.h"
class UInputMappingContext;
class UInputAction;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCallLeftBombCollision);

UCLASS()
class BOMBERMANPROTO_API ABBMCharacter : public ACharacter
{
   GENERATED_BODY()

public:
    UPROPERTY(Replicated, BlueprintReadOnly)
    int32 length = 1;
public:
    UPROPERTY()
    class UNetGameInstance* gi;

public:
   // Sets default values for this character's properties
   ABBMCharacter();

protected:
   // Called when the game starts or when spawnedrla
   virtual void BeginPlay() override;

private:
    enum ENetRole myLocalRole;
    enum ENetRole myRemoteRole;

public:
   // Called every frame
   virtual void Tick(float DeltaTime) override;

   // Called to bind functionality to input
   virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

   //enhanced Input
   UPROPERTY(EditAnywhere, Category = Input)
      class UInputMappingContext* defaultContext;
   UPROPERTY(EditAnywhere, Category = Input)
      class UInputAction* MovementAction;
   UPROPERTY(EditAnywhere, Category = Input)
      class UInputAction* SetBombAction;
   UPROPERTY(EditAnywhere, Category = Input)
      class UInputAction* BombingAction;
      UPROPERTY(EditAnywhere, Category = Input)
      class UInputAction* ChattingAction;
   //캐릭터
   //캐릭터 움직임

  
   void Move(const FInputActionValue& value);

 

   //폭탄
   //폭탄 놓는 함수 (space버튼)

   UFUNCTION(Server, Reliable, WithValidation)
    void ServerSetBomb();

//    UFUNCTION(NetMulticast, Reliable)
//    void MulticastSetBomb();

   UFUNCTION()
   void SetBomb();
   //폭탄에 맞았을 경우 실행되는 함수(푱날아가게)
   void HitBomb();
   //폭탄에 맞았을 때 날아가는 시간 (CreateTime값에 따라 시간초 조정 가능)
   float FlyCurrentTime;
   float FlyCreateTime = 3;
   //폭탄에 맞았을 때 날아가는 속도
   UPROPERTY(EditAnywhere, Category = Fly)
      float FlySpeed = 1000;
   //폭탄에 맞아서 날아가야되는지?
   UPROPERTY(EditAnywhere, Category = Fly)
      bool IsFly = false;
   //날아갈 방향
   FVector FlyDirection = FVector::UpVector + FVector::RightVector;
   //몽타주 플레이
   class UPlayerAnim* FlyMontage;

   UPROPERTY(VisibleAnywhere, Category = Fly)
      class UNiagaraComponent* blast_Particle;

   // 박스 시간 스킵
   void BoxSkipBtn();

   UFUNCTION(Server, Reliable)
   void PowerUp();

    UPROPERTY(EditAnywhere, Category="MySettings")
	float initialLength = 1;

    
   // 폭탄 잡는 함수
   void BombCatch();
   // 폭탄 놓는 함수
   void BombPut();

         // 박스 잡을 때 쓰는 콜리전
    UPROPERTY(EditAnywhere, Category = Trigger)
   class UCapsuleComponent* TriggerComp;
      // 폭탄 날아가는 속도
   UPROPERTY(EditAnywhere)
   float Bombspeed = 0.01f;
   // 폭탄 날아가는 각도
   UPROPERTY(EditAnywhere)
   float BombAngle = 75.0f;

   UFUNCTION()
   void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

   UFUNCTION()
    void OnTriggerEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

   UPROPERTY(VisibleAnywhere, Category=MySettings)
   class ABomb* ClosestBomb;

   float speed = 1000.0f; // 이동 속도

   float targetDistance = 200.0f; // 목표 이동 거리

   void Bombing();
   AActor* OverlappingActor = nullptr; // OtherActor을 저장할 변수

   bool isBombing = false;

      UPROPERTY(EditAnywhere, Category = MySettings)
   class UInputAction* IA_SetBomb;

   // 스피드 아이템 먹으면 스피드 증가하는 함수
   void SpeedUp();

   // 폭탄 개수 증가 아이템 함수
   
   void UpgradeBombCount();

   // 폭탄 오버랩/블록 이벤트 함수
    UFUNCTION()
   void ClearBombCollision();

   // 폭탄 개수 증가 아이템 전용 이벤트 함수
   UFUNCTION()
   void OnBombDestroyed(AActor* DestroyedActor);


   //폭탄 개수
   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Bomb)
   int32 bombCount = 1;


   UPROPERTY(EditAnywhere, Replicated)
   TSubclassOf<ABomb> bombActor;

   UPROPERTY(VisibleAnywhere, Category = Bomb)
    TArray<AActor*> placedBombs;

    //Die 사운드 추가
    UPROPERTY(EditAnywhere, Category = "Sound")
	class USoundWave* DieSound;
    
    //BombPut 사운드 추가
    UPROPERTY(EditAnywhere, Category = "Sound")
	class USoundWave* BombPutSound;

    bool IsGridCellOccupied(const FIntPoint& GridCell) const;
    FIntPoint GetGridCell(const FVector& WorldLocation) const;

    FCallLeftBombCollision callBombCollision;

    bool iskick= false;

    //플레이어 닉네임을 띄울 위젯컴포넌트
    UPROPERTY(VisibleAnywhere, Category=MySettings, meta=(AllowPrivateAccess="true"))
    class UWidgetComponent* infoWidget;


    UFUNCTION(Server, Reliable)
    void ServerSetInitinfo(FPlayerInitInfo InitInfo);

    void InitializePlayer();

    UPROPERTY()
	class UPlayerInfoWidget* info_UI;

    UPROPERTY(EditAnywhere, Category=MySettings)
    TSubclassOf<class UWaitingUI> waitingui;

    UPROPERTY(Replicated, BlueprintReadOnly)
    FString myName="Test Name";
   
    UPROPERTY(Replicated)
	FLinearColor myMeshColor = FLinearColor::White;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool isScoreCheck = false;
   
};