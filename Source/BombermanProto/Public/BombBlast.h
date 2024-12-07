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





	//폭발 불줄기 길이  (아이템 추가시 1레벨당 100씩 증가시켜주면 됨)
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

	//overlap되면 Bomb으로 넘길 bool값
	UPROPERTY()
	bool bIsblast;

	//태정이형 여길봐
	//박스와 닿았을 때 아이템을 스폰할 확률 변수
	UPROPERTY()
	int RandomSpawn;
	
	//스폰한다면, 어떤 아이템을 스폰할지 정하는 확률변수
	UPROPERTY()
	int Randomitem;

	
};
