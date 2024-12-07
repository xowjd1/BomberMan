// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/SkeletalMeshComponent.h>
#include <../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraComponent.h>
#include <Components/BoxComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>
#include "Bomb.generated.h"

UCLASS()
class BOMBERMANPROTO_API ABomb : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABomb();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class ABombBlast* spawnedBombBlast;

	//폭탄 메쉬
UPROPERTY(VisibleAnywhere)
class USkeletalMeshComponent* Bombmesh;

//박스 콜리전
UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
class UBoxComponent* boxcomp;

//나이아가라 파티클
UPROPERTY(VisibleAnywhere)
class UNiagaraComponent* Fuse_Particle;

//block박스 콜리전
UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
class UBoxComponent* BlockB;



//현재 시간 
UPROPERTY()
float currentTime = 0;

//blast와 폭탄이 overlap됐을 때 실행 될 함수
void Explode();

//blast와 만나면 잠깐 있다가 터지게 할 timer
UPROPERTY(EditAnywhere, Category= MySettings)
float ExplodeCurrentTime=0;

UPROPERTY(EditAnywhere, Category= MySettings)
float ExplodeCreateTime=0.1f;

//blast와 만났는가?
bool IsBlast = false;

//Make Array 부분
FVector MakeArr[4];

UPROPERTY(EditAnywhere, Category=MySettings)
TSubclassOf<class ABombBlast>blast;

//blast와 만났을 때 실행되는 Hit Event
UFUNCTION()
 void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);


//bomb 폭발 사운드 추가
UPROPERTY(EditAnywhere, Category = "Sound")
	class USoundWave* BombSound;

UFUNCTION()
void OnBombEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

void CLearBombCollision();
// 폭탄 커졌다 작아졌다하는 거 감지
bool isBombScale = false;

// 중력 작용하려고
UPROPERTY(EditAnywhere, Category=Movement)
    UProjectileMovementComponent* ProjectileMovementComponent;

	// 3초 안에 죽는 거 감지
	bool isBombDie = false;
	bool isTrigger = false;

   void BombKick(float deltaTime);
   UPROPERTY(EditAnywhere)
   float isBombSpeed = 500.0f; // 이동 속도


   FVector p0;
   FVector vt;
   FVector p;
   FVector dir;
 

   bool bPlayBombKick = false;
   void PlayBombKick();
	void getplayer();
	
	FVector startlocation;

	UPROPERTY(EditAnywhere)
	class AActor* debugActor;

	UFUNCTION(NetMulticast, Reliable)
	void PlayBomb();

	//박스와 닿았을 때 아이템을 스폰할 확률 변수
	UPROPERTY()
	int RandomSpawn;
	
	//스폰한다면, 어떤 아이템을 스폰할지 정하는 확률변수
	UPROPERTY()
	int Randomitem;

	//박스와 닿았을 때 아이템을 스폰할 확률 변수
	UPROPERTY()
	int RandomSpawn2;
	
	//스폰한다면, 어떤 아이템을 스폰할지 정하는 확률변수
	UPROPERTY()
	int Randomitem2;
};
