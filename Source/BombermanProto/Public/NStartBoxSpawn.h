// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Net/UnrealNetwork.h"
#include "NStartBoxSpawn.generated.h"

UCLASS()
class BOMBERMANPROTO_API ANStartBoxSpawn : public AActor
{
   GENERATED_BODY()
   
public:   
   // Sets default values for this actor's properties
   ANStartBoxSpawn();

protected:
   // Called when the game starts or when spawned
   virtual void BeginPlay() override;

   // 타이머 핸들
   FTimerHandle TimerHandle;

   // 타이머 함수
   void Timer60();
   UFUNCTION(NetMulticast, Reliable)
   void TimerCallback();
   UFUNCTION(Server, Reliable)
   void DestoryBoxServer(class ANBox* destroyBox);
   UFUNCTION(NetMulticast, Reliable)
   void DestoryBoxMulticast(class ANBox* destroyBox);


public:   
   // Called every frame
   virtual void Tick(float DeltaTime) override;

   // 배열 arr1의 값을 기반으로 액터를 스폰
   void BoxSpawn(int arr[19][15]);

   // 첫 번째 박스 생성
   void FirstSpawnBox();

   // 두 번째 박스 생성
   void TwoSpawnBox();

   // 세 번째 박스 생성
   void ThreeSpawnBox();

   // Replicated 사용 안 하고 ReplicatedUsing 쓰는 이유
   // 값이 변경되는 시점에서 호출되는 특정 함수를 내가 지정해 줄 수 있음 값이 동기화 되는 타이밍을 세밀하게 조절하기 위함
   UPROPERTY(ReplicatedUsing = OnRep_randomValue)
   int32 randomValue;

   UFUNCTION()
    void OnRep_randomValue();

   UFUNCTION(Server, Reliable)
    void GenerateAndReplicateRandomSeedServer();
   UFUNCTION(NetMulticast, Reliable)
    void GenerateAndReplicateRandomSeedMulti();

    virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;



   // No Box
   void BoxNoSpawn();

   //타이머 돌릴 float변수
   UPROPERTY(EditAnywhere, Category=Timer)
   float TimerCurrentTime=0;

   UPROPERTY(EditAnywhere, Category=Timer)
   float TimerCreateTime=3;
   
   //타이머가 지났는지
   UPROPERTY(EditAnywhere, Category=Timer)
   bool bIsOver= false;

   bool isFirstPlay = true;

private:
   // 배열 arr1과 arr2를 멤버 변수로 선언
   int arr1[19][15] = { 0 };

   // 박스 블루프린트 클래스를 저장하는 변수
   UPROPERTY(EditAnywhere, Category = "Box")
   TSubclassOf<class ANBox> BoxClass;

   UPROPERTY(EditAnywhere, Category = "Box")
   TSubclassOf<class ANoBox> NoBoxClass;

};