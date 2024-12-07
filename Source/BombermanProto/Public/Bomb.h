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

	//��ź �޽�
UPROPERTY(VisibleAnywhere)
class USkeletalMeshComponent* Bombmesh;

//�ڽ� �ݸ���
UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
class UBoxComponent* boxcomp;

//���̾ư��� ��ƼŬ
UPROPERTY(VisibleAnywhere)
class UNiagaraComponent* Fuse_Particle;

//block�ڽ� �ݸ���
UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
class UBoxComponent* BlockB;



//���� �ð� 
UPROPERTY()
float currentTime = 0;

//blast�� ��ź�� overlap���� �� ���� �� �Լ�
void Explode();

//blast�� ������ ��� �ִٰ� ������ �� timer
UPROPERTY(EditAnywhere, Category= MySettings)
float ExplodeCurrentTime=0;

UPROPERTY(EditAnywhere, Category= MySettings)
float ExplodeCreateTime=0.1f;

//blast�� �����°�?
bool IsBlast = false;

//Make Array �κ�
FVector MakeArr[4];

UPROPERTY(EditAnywhere, Category=MySettings)
TSubclassOf<class ABombBlast>blast;

//blast�� ������ �� ����Ǵ� Hit Event
UFUNCTION()
 void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);


//bomb ���� ���� �߰�
UPROPERTY(EditAnywhere, Category = "Sound")
	class USoundWave* BombSound;

UFUNCTION()
void OnBombEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

void CLearBombCollision();
// ��ź Ŀ���� �۾������ϴ� �� ����
bool isBombScale = false;

// �߷� �ۿ��Ϸ���
UPROPERTY(EditAnywhere, Category=Movement)
    UProjectileMovementComponent* ProjectileMovementComponent;

	// 3�� �ȿ� �״� �� ����
	bool isBombDie = false;
	bool isTrigger = false;

   void BombKick(float deltaTime);
   UPROPERTY(EditAnywhere)
   float isBombSpeed = 500.0f; // �̵� �ӵ�


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

	//�ڽ��� ����� �� �������� ������ Ȯ�� ����
	UPROPERTY()
	int RandomSpawn;
	
	//�����Ѵٸ�, � �������� �������� ���ϴ� Ȯ������
	UPROPERTY()
	int Randomitem;

	//�ڽ��� ����� �� �������� ������ Ȯ�� ����
	UPROPERTY()
	int RandomSpawn2;
	
	//�����Ѵٸ�, � �������� �������� ���ϴ� Ȯ������
	UPROPERTY()
	int Randomitem2;
};
