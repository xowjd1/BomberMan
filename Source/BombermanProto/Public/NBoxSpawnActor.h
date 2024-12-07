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

   // 타이머 핸들
   FTimerHandle TimerHandle;

   UFUNCTION(Server, Reliable)
   void spawnTimerCallback();

   // 멀티캐스트를 통해 클라이언트에게 결과 전달
UFUNCTION(NetMulticast, Reliable)
void MultiCast_SpawnTimerCallback();




public:
   // bool isBoxDestroy = false
   // Called every frame
   virtual void Tick(float DeltaTime) override;

   // 배열 arr1의 값을 기반으로 액터를 스폰
   void BoxSpawn(int arr[19][15]);

   // 배열 arr1의 값을 기반으로 액터를 스폰
   void RandBoxSpawn(int arr[19][15]);

   // 첫 번째 박스 생성
   void FirstSpawnBox();

   // 첫 번째 바깥쪽 박스 생성
   void RandomBox1SpawnBox();
   void RandomBox2SpawnBox();
   void RandomBox3SpawnBox();
   void RandomBox4SpawnBox();



   // No Box
   void BoxNoSpawn();

   void NoBoxSpawn(int arr[19][15]);


   //타이머 돌릴 float변수
   UPROPERTY(EditAnywhere, Category=Timer)
   float TimerCurrentTime=0;

   UPROPERTY(EditAnywhere, Category=Timer)
   float TimerCreateTime=5;

   

   // 박스 삭제
   UPROPERTY(EditAnywhere, Category=Timer)
   bool isBoxDestroy = true;



private:
   // 배열 arr1과 arr2를 멤버 변수로 선언
   int arr1[19][15] = { 0 };

   // 박스 블루프린트 클래스를 저장하는 변수
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