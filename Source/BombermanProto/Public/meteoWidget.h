// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "meteoWidget.generated.h"

UCLASS()
class BOMBERMANPROTO_API AmeteoWidget : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AmeteoWidget();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxComp;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* plane;

	class AMeteo* meteo;

};
