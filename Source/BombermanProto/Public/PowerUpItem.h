// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PowerUpItem.generated.h"

UCLASS()
class BOMBERMANPROTO_API APowerUpItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APowerUpItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* boxcomp;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* plane;

	UPROPERTY(VisibleAnywhere)
	class UMaterial* planemat;

	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, Category= MySettings)
	int Ishit=3;

		//ItemGet 사운드 추가
	UPROPERTY(EditAnywhere, Category = "Sound")
	class USoundWave* ItemGetSound;

};
