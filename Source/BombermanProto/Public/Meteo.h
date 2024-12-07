// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Meteo.generated.h"

UCLASS()
class BOMBERMANPROTO_API AMeteo : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMeteo();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
   // ���� �������� �����ϱ�
    UPROPERTY(EditAnywhere)
   class UBoxComponent* BoxComp;

   
   // ���� �������� �����ϱ�
    UPROPERTY(EditAnywhere)
   class UBoxComponent* DieComp;


   // �޽� ����
   UPROPERTY(EditAnywhere)
   class UStaticMeshComponent* BodyMesh;

   
   // �̵� �ӵ�
   UPROPERTY(EditAnywhere)
   float speed = 200;

public:
   void DestroyBox();

   // �ڽ� ���� �Լ��� ����ƴ���
   bool isDestroy = false;

   UFUNCTION()
	void OnComponentBeginOverlap (UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	bool bStop=false;

	//�������� �����
	//���ڰ� �ν����� ���� PowerUp ������
	UPROPERTY(EditAnywhere, Category= MySettings)
	TSubclassOf<class APowerUpItem> PowerUpItem;

	UPROPERTY(EditAnywhere, Category= MySettings)
	TSubclassOf<class ASpeedUp> SpeedUpItem;
	
	UPROPERTY(EditAnywhere, Category= MySettings)
	TSubclassOf<class AUpgradeBombCount> BombUpItem;
	

	
   //Ȯ���� ���� �������� Spawn�� �Լ�
   void SpawnItem(int val, FVector location);

};
