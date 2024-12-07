// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UpgradeBombCount.generated.h"

UCLASS()
class BOMBERMANPROTO_API AUpgradeBombCount : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUpgradeBombCount();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* boxcomp;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* bombItem;

	UFUNCTION()
	void OnComponentBeginOverlap (UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

		//ItemGet 사운드 추가
	UPROPERTY(EditAnywhere, Category = "Sound")
	class USoundWave* ItemGetSound;

};
