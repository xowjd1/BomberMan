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
   //ĳ����
   //ĳ���� ������

  
   void Move(const FInputActionValue& value);

 

   //��ź
   //��ź ���� �Լ� (space��ư)

   UFUNCTION(Server, Reliable, WithValidation)
    void ServerSetBomb();

//    UFUNCTION(NetMulticast, Reliable)
//    void MulticastSetBomb();

   UFUNCTION()
   void SetBomb();
   //��ź�� �¾��� ��� ����Ǵ� �Լ�(�y���ư���)
   void HitBomb();
   //��ź�� �¾��� �� ���ư��� �ð� (CreateTime���� ���� �ð��� ���� ����)
   float FlyCurrentTime;
   float FlyCreateTime = 3;
   //��ź�� �¾��� �� ���ư��� �ӵ�
   UPROPERTY(EditAnywhere, Category = Fly)
      float FlySpeed = 1000;
   //��ź�� �¾Ƽ� ���ư��ߵǴ���?
   UPROPERTY(EditAnywhere, Category = Fly)
      bool IsFly = false;
   //���ư� ����
   FVector FlyDirection = FVector::UpVector + FVector::RightVector;
   //��Ÿ�� �÷���
   class UPlayerAnim* FlyMontage;

   UPROPERTY(VisibleAnywhere, Category = Fly)
      class UNiagaraComponent* blast_Particle;

   // �ڽ� �ð� ��ŵ
   void BoxSkipBtn();

   UFUNCTION(Server, Reliable)
   void PowerUp();

    UPROPERTY(EditAnywhere, Category="MySettings")
	float initialLength = 1;

    
   // ��ź ��� �Լ�
   void BombCatch();
   // ��ź ���� �Լ�
   void BombPut();

         // �ڽ� ���� �� ���� �ݸ���
    UPROPERTY(EditAnywhere, Category = Trigger)
   class UCapsuleComponent* TriggerComp;
      // ��ź ���ư��� �ӵ�
   UPROPERTY(EditAnywhere)
   float Bombspeed = 0.01f;
   // ��ź ���ư��� ����
   UPROPERTY(EditAnywhere)
   float BombAngle = 75.0f;

   UFUNCTION()
   void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

   UFUNCTION()
    void OnTriggerEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

   UPROPERTY(VisibleAnywhere, Category=MySettings)
   class ABomb* ClosestBomb;

   float speed = 1000.0f; // �̵� �ӵ�

   float targetDistance = 200.0f; // ��ǥ �̵� �Ÿ�

   void Bombing();
   AActor* OverlappingActor = nullptr; // OtherActor�� ������ ����

   bool isBombing = false;

      UPROPERTY(EditAnywhere, Category = MySettings)
   class UInputAction* IA_SetBomb;

   // ���ǵ� ������ ������ ���ǵ� �����ϴ� �Լ�
   void SpeedUp();

   // ��ź ���� ���� ������ �Լ�
   
   void UpgradeBombCount();

   // ��ź ������/��� �̺�Ʈ �Լ�
    UFUNCTION()
   void ClearBombCollision();

   // ��ź ���� ���� ������ ���� �̺�Ʈ �Լ�
   UFUNCTION()
   void OnBombDestroyed(AActor* DestroyedActor);


   //��ź ����
   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Bomb)
   int32 bombCount = 1;


   UPROPERTY(EditAnywhere, Replicated)
   TSubclassOf<ABomb> bombActor;

   UPROPERTY(VisibleAnywhere, Category = Bomb)
    TArray<AActor*> placedBombs;

    //Die ���� �߰�
    UPROPERTY(EditAnywhere, Category = "Sound")
	class USoundWave* DieSound;
    
    //BombPut ���� �߰�
    UPROPERTY(EditAnywhere, Category = "Sound")
	class USoundWave* BombPutSound;

    bool IsGridCellOccupied(const FIntPoint& GridCell) const;
    FIntPoint GetGridCell(const FVector& WorldLocation) const;

    FCallLeftBombCollision callBombCollision;

    bool iskick= false;

    //�÷��̾� �г����� ��� ����������Ʈ
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