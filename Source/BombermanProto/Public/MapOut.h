// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MapOut.generated.h"

UCLASS()
class BOMBERMANPROTO_API AMapOut : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMapOut();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* mapOutMesh;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* floorMesh;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* mapboxcomp1;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* mapboxcomp2;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* mapboxcomp3;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* mapboxcomp4;
};
