// Fill out your copyright notice in the Description page of Project Settings.

#include "NBoxSpawnActor.h"
#include "Meteo.h"
#include "MeteoWidget.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"
#include "NoBox.h"
#include "kismet/GameplayStatics.h"
#include "NStartBoxSpawn.h"
#include "NBox.h"


// Sets default values
ANBoxSpawnActor::ANBoxSpawnActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;



	bReplicates = true;
	SetReplicateMovement(true);
}

// Called when the game starts or when spawned
void ANBoxSpawnActor::BeginPlay()
{
	Super::BeginPlay();


	if (HasAuthority())
	{
		auto nboxSpawner = UGameplayStatics::GetActorOfClass(GetWorld(), ANBoxSpawnActor::StaticClass());
		if (nboxSpawner)
		{
			//nboxSpawner->SetOwner(this);
		}
	}
}

// Called every frame
void ANBoxSpawnActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isBoxDestroy == true)
	{

		// �����϶� ó������
		if (HasAuthority())
		{
			// Get the current network time (in seconds)
			float NetworkTime = GetWorld()->GetTimeSeconds();

			TimerCurrentTime += DeltaTime;
			if (TimerCurrentTime > TimerCreateTime)
			{
				TimerCurrentTime = 0;
				//bIsOver = false;
				// Call the timer callback

				spawnTimerCallback();
				isBoxDestroy = false;

			}

		}
	}
}


void ANBoxSpawnActor::spawnTimerCallback_Implementation()
{

	MultiCast_SpawnTimerCallback();
	FirstSpawnBox();
	BoxNoSpawn();

	RandomBox1SpawnBox();
	RandomBox2SpawnBox();
	RandomBox3SpawnBox();
	RandomBox4SpawnBox();
}

void ANBoxSpawnActor::MultiCast_SpawnTimerCallback_Implementation()
{
}


//
//void ANBoxSpawnActor::MultiCast_SpawnTimerCallback_Implementation()
//{
//
//}

//
//void ANBoxSpawnActor::spawnTimerCallback()
//{
//
//
//}



// �迭 arr1�� ���� ������� ���͸� ����
void ANBoxSpawnActor::BoxSpawn(int arr[19][15])
{

	for (int row = 0; row < 19; row++)
	{
		for (int col = 0; col < 15; col++)
		{

			if (arr[row][col] == 3)
			{
				FVector SpawnLocation = FVector(row * 100, col * 100, 1000);
				FVector meteoLocation = FVector(row * 100, col * 100, 30);
				AActor* SpawnActor = GetWorld()->SpawnActor<AActor>(BoxClass, SpawnLocation, FRotator::ZeroRotator);
				AActor* meteoSpawn = GetWorld()->SpawnActor<AActor>(MeteoWidget, meteoLocation, FRotator::ZeroRotator);

			}


		}
	}
}

void ANBoxSpawnActor::RandBoxSpawn(int arr[19][15])
{

	for (int row = 0; row < 19; row++)
	{
		for (int col = 0; col < 15; col++)
		{
			if (arr[row][col] == 1)
			{
				// 1�� ��ǥ�� ������ ����
				// �ڽ��� ����
				FVector SpawnLocation = FVector(row * 100, col * 100, 1000);
				FVector meteoLocation = FVector(row * 100, col * 100, 30);
				AActor* SpawnActor = GetWorld()->SpawnActor<AActor>(BoxClass, SpawnLocation, FRotator::ZeroRotator);
				AActor* meteoSpawn = GetWorld()->SpawnActor<AActor>(MeteoWidget, meteoLocation, FRotator::ZeroRotator);

			}

		}
	}
}


// ù ��° �ڽ� ����
void ANBoxSpawnActor::FirstSpawnBox()
{

#pragma region
	arr1[5][2] = 1;
	arr1[6][2] = 1;
	arr1[7][2] = 1;
	arr1[9][2] = 1;
	arr1[11][2] = 1;
	arr1[12][2] = 1;
	arr1[13][2] = 1;


	arr1[4][3] = 1;
	arr1[14][3] = 1;

	arr1[4][4] = 1;
	arr1[6][4] = 1;
	arr1[7][4] = 1;
	arr1[8][4] = 1;
	arr1[9][4] = 1;
	arr1[10][4] = 1;
	arr1[11][4] = 1;
	arr1[12][4] = 1;
	arr1[14][4] = 1;

	arr1[4][5] = 1;
	arr1[6][5] = 1;
	arr1[12][5] = 1;
	arr1[14][5] = 1;

	arr1[6][6] = 1;
	arr1[12][6] = 1;

	arr1[4][7] = 1;
	arr1[6][7] = 1;
	arr1[12][7] = 1;
	arr1[14][7] = 1;

	arr1[6][8] = 1;
	arr1[12][8] = 1;

	arr1[4][9] = 1;
	arr1[6][9] = 1;
	arr1[12][9] = 1;
	arr1[14][9] = 1;

	arr1[4][10] = 1;
	arr1[6][10] = 1;
	arr1[7][10] = 1;
	arr1[8][10] = 1;
	arr1[9][10] = 1;
	arr1[10][10] = 1;
	arr1[11][10] = 1;
	arr1[12][10] = 1;
	arr1[14][10] = 1;

	arr1[4][11] = 1;
	arr1[14][11] = 1;

	arr1[5][12] = 1;
	arr1[6][12] = 1;
	arr1[7][12] = 1;
	arr1[9][12] = 1;
	arr1[11][12] = 1;
	arr1[12][12] = 1;
	arr1[13][12] = 1;


#pragma endregion (arr)
	BoxSpawn(arr1);

}

// ù ��° �ٱ��� �ڽ� ����
void ANBoxSpawnActor::RandomBox1SpawnBox()
{
	// 4���ҷ� ������ �� ����
	for (int row = 0; row < 8; row++)
	{
		for (int col = 0; col < 6; col++)
		{
			if (arr1[row][col] != 1 && arr1[row][col] != 2 && arr1[row][col] != 4)
			{
				arr1[row][col] = 3;
			}

			if (arr1[row][col] == 3)
			{
				// 3�� ��� 50% Ȯ���� 3�� ��ǥ�� �������� ����
				if (FMath::RandBool() && count1 < 7) // 50%�� Ȯ���� �� �Ǵ� ���� ��ȯ
				{
					FVector SpawnLocation = FVector(row * 100, col * 100, 1000);
					FVector meteoLocation = FVector(row * 100, col * 100, 30);
					AActor* SpawnActor = GetWorld()->SpawnActor<AActor>(BoxClass, SpawnLocation, FRotator::ZeroRotator);
					AActor* meteoSpawn = GetWorld()->SpawnActor<AActor>(MeteoWidget, meteoLocation, FRotator::ZeroRotator);

					count1++;
				}
			}
		}
	}

	RandBoxSpawn(arr1);
}


void ANBoxSpawnActor::RandomBox2SpawnBox()
{
	for (int row = 0; row < 8; row++)
	{
		for (int col = 7; col < 14; col++)
		{
			if (arr1[row][col] != 1 && arr1[row][col] != 2 && arr1[row][col] != 4)
			{
				arr1[row][col] = 3;
			}

			if (arr1[row][col] == 3)
			{
				// 3�� ��� 50% Ȯ���� 3�� ��ǥ�� �������� ����
				if (FMath::RandBool() && count2 < 7) // 50%�� Ȯ���� �� �Ǵ� ���� ��ȯ
				{
					FVector SpawnLocation = FVector(row * 100, col * 100, 1000);
					FVector meteoLocation = FVector(row * 100, col * 100, 30);
					AActor* SpawnActor = GetWorld()->SpawnActor<AActor>(BoxClass, SpawnLocation, FRotator::ZeroRotator);
					AActor* meteoSpawn = GetWorld()->SpawnActor<AActor>(MeteoWidget, meteoLocation, FRotator::ZeroRotator);

					count2++;
				}
			}
		}
	}
}

void ANBoxSpawnActor::RandomBox3SpawnBox()
{

	for (int row = 9; row < 18; row++)
	{
		for (int col = 0; col < 6; col++)
		{
			if (arr1[row][col] != 1 && arr1[row][col] != 2 && arr1[row][col] != 4)
			{
				arr1[row][col] = 3;
			}

			if (arr1[row][col] == 3)
			{
				// 3�� ��� 50% Ȯ���� 3�� ��ǥ�� �������� ����
				if (FMath::RandBool() && count3 < 7) // 50%�� Ȯ���� �� �Ǵ� ���� ��ȯ
				{
					FVector SpawnLocation = FVector(row * 100, col * 100, 1000);
					FVector meteoLocation = FVector(row * 100, col * 100, 30);
					AActor* SpawnActor = GetWorld()->SpawnActor<AActor>(BoxClass, SpawnLocation, FRotator::ZeroRotator);
					AActor* meteoSpawn = GetWorld()->SpawnActor<AActor>(MeteoWidget, meteoLocation, FRotator::ZeroRotator);

					count3++;
				}
			}
		}
	}
}

void ANBoxSpawnActor::RandomBox4SpawnBox()
{
	for (int row = 9; row < 18; row++)
	{
		for (int col = 7; col < 14; col++)
		{
			if (arr1[row][col] != 1 && arr1[row][col] != 2 && arr1[row][col] != 4)
			{
				arr1[row][col] = 3;
			}

			if (arr1[row][col] == 3)
			{
				// 3�� ��� 50% Ȯ���� 3�� ��ǥ�� �������� ����
				if (FMath::RandBool() && count4 < 7) // 50%�� Ȯ���� �� �Ǵ� ���� ��ȯ
				{
					FVector SpawnLocation = FVector(row * 100, col * 100, 1000);
					FVector meteoLocation = FVector(row * 100, col * 100, 30);
					AActor* SpawnActor = GetWorld()->SpawnActor<AActor>(BoxClass, SpawnLocation, FRotator::ZeroRotator);
					AActor* meteoSpawn = GetWorld()->SpawnActor<AActor>(MeteoWidget, meteoLocation, FRotator::ZeroRotator);

					count4++;
				}
			}
		}
	}

}

// No Box
void ANBoxSpawnActor::BoxNoSpawn()
{
	// ö �ڽ�
	for (int row = 0; row < 19; row++)
	{
		for (int col = 0; col < 15; col++)
		{

			// Ȧ�� ���� �ִ� ������ ���� ���� X
			if (col % 2 == 1 && row % 2 == 1)
			{
				arr1[row][col] = 2;
			}
		}
	}

	// �ϴû� �ڽ�
	arr1[8][5] = 4;
	arr1[10][5] = 4;

	for (int row = 7; row <= 11; row++)
	{
		arr1[row][6] = 4;
	}

	arr1[8][7] = 4;
	arr1[10][7] = 4;

	for (int row = 7; row <= 11; row++)
	{
		arr1[row][8] = 4;
	}

	arr1[8][9] = 4;
	arr1[10][9] = 4;

	// �׽�Ʈ
	//NoBoxSpawn(arr1);
}

// ö �ڽ� ����(�ҷ����� �������Ʈ�� �޶�)
void ANBoxSpawnActor::NoBoxSpawn(int arr[19][15])
{
	for (int row = 0; row < 19; row++)
	{
		for (int col = 0; col < 15; col++)
		{

			if (arr[row][col] == 2)
			{
				FVector SpawnLocation = FVector(row * 100, col * 100, 1000);
				FVector meteoLocation = FVector(row * 100, col * 100, 30);
				AActor* SpawnActor = GetWorld()->SpawnActor<AActor>(BoxClass, SpawnLocation, FRotator::ZeroRotator);
				AActor* meteoSpawn = GetWorld()->SpawnActor<AActor>(MeteoWidget, meteoLocation, FRotator::ZeroRotator);
			}


		}
	}
}
