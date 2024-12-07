// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NoBox.generated.h"

UCLASS()
class BOMBERMANPROTO_API ANoBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANoBox();

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
};
