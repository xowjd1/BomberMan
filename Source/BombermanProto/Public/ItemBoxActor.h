// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemBoxActor.generated.h"

UCLASS()
class BOMBERMANPROTO_API AItemBoxActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemBoxActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// *** 임의로 아이템 하나만
	UPROPERTY(EditAnywhere, Category = "ItemBox")
	TSubclassOf<class AItemOne> ItemClass;

	void RandItem();

};
