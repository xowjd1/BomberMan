// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BombBlast.generated.h"

UCLASS()
class BOMBERMANPROTO_API ABombBlast : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABombBlast();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	//UFUNCTION(NetMulticast, Reliable)
	//void PlayBomb(class ABBMCharacter* player);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	class USphereComponent* blastComp;

	UPROPERTY(VisibleAnywhere)
	class UNiagaraComponent* blast_Particle;

	UPROPERTY(EditAnywhere, Category="Timer")
	float createTime = 0.5f;

	UPROPERTY()
	float currentTime = 0;





	//���� ���ٱ� ����  (������ �߰��� 1������ 100�� ���������ָ� ��)
	//UPROPERTY(EditAnywhere, Category="bombSettings")
	//inline static float length = 0;
	UPROPERTY(EditAnywhere, Category="bombSettings")
	FVector StartLocation;

	UPROPERTY()
	class ANBox* box;

	UPROPERTY()
	class ABombBlast* me;


	UFUNCTION()
	void OnComponentBeginOverlap (UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//overlap�Ǹ� Bomb���� �ѱ� bool��
	UPROPERTY()
	bool bIsblast;

	//�������� �����
	//�ڽ��� ����� �� �������� ������ Ȯ�� ����
	UPROPERTY()
	int RandomSpawn;
	
	//�����Ѵٸ�, � �������� �������� ���ϴ� Ȯ������
	UPROPERTY()
	int Randomitem;

	
};
