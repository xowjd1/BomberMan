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

   // Ÿ�̸� �ڵ�
   FTimerHandle TimerHandle;

   // Ÿ�̸� �Լ�
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

   // �迭 arr1�� ���� ������� ���͸� ����
   void BoxSpawn(int arr[19][15]);

   // ù ��° �ڽ� ����
   void FirstSpawnBox();

   // �� ��° �ڽ� ����
   void TwoSpawnBox();

   // �� ��° �ڽ� ����
   void ThreeSpawnBox();

   // Replicated ��� �� �ϰ� ReplicatedUsing ���� ����
   // ���� ����Ǵ� �������� ȣ��Ǵ� Ư�� �Լ��� ���� ������ �� �� ���� ���� ����ȭ �Ǵ� Ÿ�̹��� �����ϰ� �����ϱ� ����
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

   //Ÿ�̸� ���� float����
   UPROPERTY(EditAnywhere, Category=Timer)
   float TimerCurrentTime=0;

   UPROPERTY(EditAnywhere, Category=Timer)
   float TimerCreateTime=3;
   
   //Ÿ�̸Ӱ� ��������
   UPROPERTY(EditAnywhere, Category=Timer)
   bool bIsOver= false;

   bool isFirstPlay = true;

private:
   // �迭 arr1�� arr2�� ��� ������ ����
   int arr1[19][15] = { 0 };

   // �ڽ� �������Ʈ Ŭ������ �����ϴ� ����
   UPROPERTY(EditAnywhere, Category = "Box")
   TSubclassOf<class ANBox> BoxClass;

   UPROPERTY(EditAnywhere, Category = "Box")
   TSubclassOf<class ANoBox> NoBoxClass;

};