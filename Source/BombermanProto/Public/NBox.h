// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NBox.generated.h"

UCLASS()
class BOMBERMANPROTO_API ANBox : public AActor
{
   GENERATED_BODY()
   
public:   
   // Sets default values for this actor's properties
   ANBox();

protected:
   // Called when the game starts or when spawned
   virtual void BeginPlay() override;

public:   
   // Called every frame
   virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere)
   // 벽돌 랜덤으로 생성하기
   class UBoxComponent* BoxComp;
   UPROPERTY(EditAnywhere)
   // 메쉬 생성
   class UStaticMeshComponent* BodyMesh;


   UPROPERTY(EditAnywhere)
   // 이동 속도
   float speed = 500;

public:
   void DestroyBox();

   // 박스 삭제 함수가 실행됐는지
   bool isDestroy = false;

   UFUNCTION()
	void OnComponentBeginOverlap (UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	bool bStop=false;

	//태정이형 여길봐
	//상자가 부숴지면 나올 PowerUp 아이템
	UPROPERTY(EditAnywhere, Category= MySettings)
	TSubclassOf<class APowerUpItem> PowerUpItem;
	
	UPROPERTY(EditAnywhere, Category= MySettings)
	TSubclassOf<class ASpeedUp> SpeedUpItem;
	
	UPROPERTY(EditAnywhere, Category= MySettings)
	TSubclassOf<class AUpgradeBombCount> BombUpItem;
	
   //확률에 따라 아이템을 Spawn할 함수
   void SpawnItem(int val, FVector location);

};