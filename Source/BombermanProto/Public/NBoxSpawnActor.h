// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NBoxSpawnActor.generated.h"

UCLASS()
class BOMBERMANPROTO_API ANBoxSpawnActor : public AActor
{
   GENERATED_BODY()

public:
   // Sets default values for this actor's properties
   ANBoxSpawnActor();

protected:
   // Called when the game starts or when spawned
   virtual void BeginPlay() override;

   // Ÿ�̸� �ڵ�
   FTimerHandle TimerHandle;

   UFUNCTION(Server, Reliable)
   void spawnTimerCallback();

   // ��Ƽĳ��Ʈ�� ���� Ŭ���̾�Ʈ���� ��� ����
UFUNCTION(NetMulticast, Reliable)
void MultiCast_SpawnTimerCallback();




public:
   // bool isBoxDestroy = false
   // Called every frame
   virtual void Tick(float DeltaTime) override;

   // �迭 arr1�� ���� ������� ���͸� ����
   void BoxSpawn(int arr[19][15]);

   // �迭 arr1�� ���� ������� ���͸� ����
   void RandBoxSpawn(int arr[19][15]);

   // ù ��° �ڽ� ����
   void FirstSpawnBox();

   // ù ��° �ٱ��� �ڽ� ����
   void RandomBox1SpawnBox();
   void RandomBox2SpawnBox();
   void RandomBox3SpawnBox();
   void RandomBox4SpawnBox();



   // No Box
   void BoxNoSpawn();

   void NoBoxSpawn(int arr[19][15]);


   //Ÿ�̸� ���� float����
   UPROPERTY(EditAnywhere, Category=Timer)
   float TimerCurrentTime=0;

   UPROPERTY(EditAnywhere, Category=Timer)
   float TimerCreateTime=5;

   

   // �ڽ� ����
   UPROPERTY(EditAnywhere, Category=Timer)
   bool isBoxDestroy = true;



private:
   // �迭 arr1�� arr2�� ��� ������ ����
   int arr1[19][15] = { 0 };

   // �ڽ� �������Ʈ Ŭ������ �����ϴ� ����
   UPROPERTY(EditAnywhere, Category = "Box")
   TSubclassOf<class AMeteo> BoxClass;

   UPROPERTY(EditAnywhere, Category = "Box")
   TSubclassOf<class ANoBox> NoBoxClass;

     UPROPERTY(EditAnywhere, Category = "Box")
   TSubclassOf<class AmeteoWidget> MeteoWidget;

   UPROPERTY(EditAnywhere)
      bool isCheck = false;


   int count1 = 0;
   int count2 = 0;
   int count3 = 0;
   int count4 = 0;


};