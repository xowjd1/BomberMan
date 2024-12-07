// Fill out your copyright notice in the Description page of Project Settings.


#include "NStartBoxSpawn.h"
#include "NBox.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"
#include "NoBox.h"
#include "Net/UnrealNetwork.h"
#include "kismet/GameplayStatics.h"

// Sets default values
ANStartBoxSpawn::ANStartBoxSpawn()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
}

// Called when the game starts or when spawned
void ANStartBoxSpawn::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("ANStartBoxSpawn Beginplay"));
	if (HasAuthority()) // 이 객체가 서버인지 확인
	{
		GenerateAndReplicateRandomSeedServer();
		GenerateAndReplicateRandomSeedMulti();
	}
	BoxNoSpawn();
}


void ANStartBoxSpawn::GenerateAndReplicateRandomSeedServer_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("SSSSSSSSSSSSSSSSSSSSSSSS"));
	randomValue = FMath::RandRange(0, 2); // 랜덤 값 생성
}

void ANStartBoxSpawn::GenerateAndReplicateRandomSeedMulti_Implementation()
{
	auto pc = GetWorld()->GetFirstPlayerController();
	UE_LOG(LogTemp, Warning, TEXT("GenerateAndReplicateRandomSeedMulti : %d, netmode : %d"), randomValue, pc->GetNetMode());
	if (randomValue == 0)
		FirstSpawnBox();
	else if (randomValue == 1)
		TwoSpawnBox();
	else
		ThreeSpawnBox();


}

// Called every frame
void ANStartBoxSpawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 서버일때 처리하자
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
	            TimerCallback();
	            isFirstPlay = false;
	            // Replicate the timer event using a network timestamp
	            //MultiCast_ReplicateTimerEvent(NetworkTime);
	        }
	    
	}
}


void ANStartBoxSpawn::Timer60()
{
	bIsOver = true;
}

void ANStartBoxSpawn::TimerCallback_Implementation()
{
	auto pc = GetWorld()->GetFirstPlayerController();
	UE_LOG(LogTemp, Warning, TEXT(":TimerCallback_Implementation :netmode : %d"), pc->GetNetMode());

	TArray<AActor*> boxes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANBox::StaticClass(), boxes);
	for (auto box : boxes)
	{
		box->Destroy();
	}

	for (int row = 0; row < 19; row++)
	{
		for (int col = 0; col < 15; col++)
		{
			if (arr1[row][col] == 1)
			{
				ANBox* NewBox = Cast<ANBox>(UGameplayStatics::GetActorOfClass(GetWorld(), ANBox::StaticClass()));
				if (NewBox && IsValid(NewBox))
				{
					arr1[row][col] = 0;
					//DestoryBoxServer(NewBox);
					// 재생성 막으려고
					bIsOver = false;
				}
			}

		}
	}
}

void ANStartBoxSpawn::DestoryBoxServer_Implementation(ANBox* destroyBox)
{
	auto pc = GetWorld()->GetFirstPlayerController();
	FString name = destroyBox ? destroyBox->GetName() : TEXT("NULL");
	UE_LOG(LogTemp, Warning, TEXT(":DestoryBoxServer : %s, netmode : %d"), *name, pc->GetNetMode());

	DestoryBoxMulticast(destroyBox);
}

void ANStartBoxSpawn::DestoryBoxMulticast_Implementation(class ANBox* destroyBox)
{
	auto pc = GetWorld()->GetFirstPlayerController();
	FString name = destroyBox ? destroyBox->GetName() : TEXT("NULL");
	if (destroyBox == nullptr)
		return;
	UE_LOG(LogTemp, Warning, TEXT("::DestoryBoxMulticast : %s, netmode : %d"), *name, pc->GetNetMode());
	destroyBox->Destroy();
}

// 배열 arr1의 값을 기반으로 액터를 스폰
void ANStartBoxSpawn::BoxSpawn(int arr[19][15])
{
	auto pc = GetWorld()->GetFirstPlayerController();
	UE_LOG(LogTemp, Warning, TEXT("::BoxSpawn netmode : %d"), pc->GetNetMode());

	for (int row = 0; row < 19; row++)
	{
		for (int col = 0; col < 15; col++)
		{
			if (arr[row][col] == 1)
			{
				FVector SpawnLocation = FVector(row * 100, col * 100, 100);
				AActor* SpawnActor = GetWorld()->SpawnActor<AActor>(BoxClass, SpawnLocation, FRotator::ZeroRotator);
			}



		}
	}
}

// 첫 번째 박스 생성
void ANStartBoxSpawn::FirstSpawnBox()
{
	UE_LOG(LogTemp, Warning, TEXT("FirstSpawnBox"));
#pragma region
	arr1[0][0] = 1;
	arr1[2][0] = 1;
	arr1[3][0] = 1;
	arr1[7][0] = 1;
	arr1[8][0] = 1;
	arr1[10][0] = 1;
	arr1[12][0] = 1;
	arr1[13][0] = 1;
	arr1[17][0] = 1;
	arr1[18][0] = 1;


	arr1[0][1] = 1;
	arr1[2][1] = 1;
	arr1[6][1] = 1;
	arr1[10][1] = 1;
	arr1[16][1] = 1;

	arr1[0][2] = 1;
	arr1[2][2] = 1;
	arr1[4][2] = 1;
	arr1[5][2] = 1;
	arr1[6][2] = 1;
	arr1[8][2] = 1;
	arr1[9][2] = 1;
	arr1[10][2] = 1;
	arr1[11][2] = 1;
	arr1[12][2] = 1;
	arr1[14][2] = 1;
	arr1[15][2] = 1;
	arr1[16][2] = 1;
	arr1[18][2] = 1;

	arr1[0][3] = 1;
	arr1[2][3] = 1;
	arr1[10][3] = 1;

	arr1[0][4] = 1;
	arr1[2][4] = 1;
	arr1[4][4] = 1;
	arr1[6][4] = 1;
	arr1[8][4] = 1;
	arr1[9][4] = 1;
	arr1[10][4] = 1;
	arr1[12][4] = 1;
	arr1[13][4] = 1;
	arr1[15][4] = 1;
	arr1[18][4] = 1;

	arr1[0][5] = 1;
	arr1[4][5] = 1;
	arr1[12][5] = 1;
	arr1[16][5] = 1;
	arr1[18][5] = 1;

	arr1[1][6] = 1;
	arr1[2][6] = 1;
	arr1[4][6] = 1;
	arr1[6][6] = 1;
	arr1[12][6] = 1;
	arr1[16][6] = 1;

	arr1[2][7] = 1;
	arr1[4][7] = 1;
	arr1[6][7] = 1;
	arr1[12][7] = 1;
	arr1[14][7] = 1;
	arr1[16][7] = 1;

	arr1[2][8] = 1;
	arr1[6][8] = 1;
	arr1[12][8] = 1;
	arr1[13][8] = 1;
	arr1[17][8] = 1;

	arr1[0][9] = 1;
	arr1[2][9] = 1;
	arr1[4][9] = 1;
	arr1[16][9] = 1;
	arr1[18][9] = 1;

	arr1[0][10] = 1;
	arr1[2][10] = 1;
	arr1[4][10] = 1;
	arr1[6][10] = 1;
	arr1[8][10] = 1;
	arr1[9][10] = 1;
	arr1[10][10] = 1;
	arr1[12][10] = 1;
	arr1[14][10] = 1;
	arr1[15][10] = 1;

	arr1[2][11] = 1;
	arr1[4][11] = 1;
	arr1[6][11] = 1;
	arr1[8][11] = 1;
	arr1[14][11] = 1;
	arr1[16][11] = 1;
	arr1[18][11] = 1;

	arr1[0][12] = 1;
	arr1[1][12] = 1;
	arr1[3][12] = 1;
	arr1[4][12] = 1;
	arr1[8][12] = 1;
	arr1[10][12] = 1;
	arr1[12][12] = 1;
	arr1[13][12] = 1;
	arr1[14][12] = 1;
	arr1[15][12] = 1;
	arr1[18][12] = 1;

	arr1[2][13] = 1;
	arr1[6][13] = 1;
	arr1[8][13] = 1;
	arr1[12][13] = 1;
	arr1[18][13] = 1;

	arr1[0][14] = 1;
	arr1[1][14] = 1;
	arr1[5][14] = 1;
	arr1[6][14] = 1;
	arr1[8][14] = 1;
	arr1[10][14] = 1;
	arr1[11][14] = 1;
	arr1[15][14] = 1;
	arr1[16][14] = 1;
	arr1[18][14] = 1;
#pragma endregion (arr)
	BoxSpawn(arr1);

}

void ANStartBoxSpawn::TwoSpawnBox()
{

	UE_LOG(LogTemp, Warning, TEXT("TwoSpawnBox"));
#pragma region
	arr1[0][0] = 1;
	arr1[2][0] = 1;
	arr1[3][0] = 1;
	arr1[7][0] = 1;
	arr1[8][0] = 1;
	arr1[10][0] = 1;
	arr1[12][0] = 1;
	arr1[13][0] = 1;
	arr1[17][0] = 1;
	arr1[18][0] = 1;

	arr1[0][1] = 1;
	arr1[6][1] = 1;
	arr1[8][1] = 1;
	arr1[10][1] = 1;
	arr1[16][1] = 1;

	arr1[0][2] = 1;
	arr1[2][2] = 1;
	arr1[3][2] = 1;
	arr1[4][2] = 1;
	arr1[6][2] = 1;
	arr1[7][2] = 1;
	arr1[8][2] = 1;
	arr1[10][2] = 1;
	arr1[12][2] = 1;
	arr1[14][2] = 1;
	arr1[16][2] = 1;
	arr1[17][2] = 1;
	arr1[18][2] = 1;

	arr1[8][3] = 1;
	arr1[12][3] = 1;

	arr1[0][4] = 1;
	arr1[2][4] = 1;
	arr1[4][4] = 1;
	arr1[5][4] = 1;
	arr1[6][4] = 1;
	arr1[8][4] = 1;
	arr1[9][4] = 1;
	arr1[10][4] = 1;
	arr1[12][4] = 1;
	arr1[14][4] = 1;
	arr1[15][4] = 1;
	arr1[16][4] = 1;
	arr1[18][4] = 1;

	arr1[0][5] = 1;
	arr1[2][5] = 1;
	arr1[18][5] = 1;

	arr1[1][6] = 1;
	arr1[2][6] = 1;
	arr1[4][6] = 1;
	arr1[6][6] = 1;
	arr1[1][6] = 1;
	arr1[12][6] = 1;
	arr1[14][6] = 1;
	arr1[16][6] = 1;

	arr1[4][7] = 1;
	arr1[6][7] = 1;
	arr1[12][7] = 1;
	arr1[14][7] = 1;

	arr1[2][8] = 1;
	arr1[4][8] = 1;
	arr1[6][8] = 1;
	arr1[12][8] = 1;
	arr1[14][8] = 1;
	arr1[17][8] = 1;

	arr1[0][9] = 1;
	arr1[18][9] = 1;

	arr1[0][10] = 1;
	arr1[2][10] = 1;
	arr1[3][10] = 1;
	arr1[4][10] = 1;
	arr1[6][10] = 1;
	arr1[8][10] = 1;
	arr1[9][10] = 1;
	arr1[10][10] = 1;
	arr1[12][10] = 1;
	arr1[14][10] = 1;
	arr1[16][10] = 1;
	arr1[18][10] = 1;

	arr1[6][11] = 1;
	arr1[12][11] = 1;
	arr1[14][11] = 1;
	arr1[16][11] = 1;

	arr1[0][12] = 1;
	arr1[1][12] = 1;
	arr1[2][12] = 1;
	arr1[4][12] = 1;
	arr1[6][12] = 1;
	arr1[8][12] = 1;
	arr1[9][12] = 1;
	arr1[10][12] = 1;
	arr1[11][12] = 1;
	arr1[12][12] = 1;
	arr1[14][12] = 1;
	arr1[16][12] = 1;
	arr1[18][12] = 1;

	arr1[2][13] = 1;
	arr1[8][13] = 1;
	arr1[12][13] = 1;
	arr1[18][13] = 1;

	arr1[0][14] = 1;
	arr1[1][14] = 1;
	arr1[5][14] = 1;
	arr1[6][14] = 1;
	arr1[8][14] = 1;
	arr1[10][14] = 1;
	arr1[11][14] = 1;
	arr1[15][14] = 1;
	arr1[16][14] = 1;
	arr1[18][14] = 1;

#pragma endregion (arr)
	BoxSpawn(arr1);

}

void ANStartBoxSpawn::ThreeSpawnBox()
{
#pragma region
	arr1[0][0] = 1;
	arr1[2][0] = 1;
	arr1[3][0] = 1;
	arr1[7][0] = 1;
	arr1[8][0] = 1;
	arr1[10][0] = 1;
	arr1[12][0] = 1;
	arr1[13][0] = 1;
	arr1[17][0] = 1;
	arr1[18][0] = 1;

	arr1[0][1] = 1;
	arr1[2][1] = 1;
	arr1[6][1] = 1;
	arr1[8][1] = 1;
	arr1[10][1] = 1;
	arr1[12][1] = 1;
	arr1[16][1] = 1;
	arr1[18][1] = 1;

	arr1[0][2] = 1;
	arr1[2][2] = 1;
	arr1[4][2] = 1;
	arr1[6][2] = 1;
	arr1[8][2] = 1;
	arr1[10][2] = 1;
	arr1[12][2] = 1;
	arr1[14][2] = 1;
	arr1[18][2] = 1;

	arr1[6][3] = 1;
	arr1[10][3] = 1;
	arr1[12][3] = 1;
	arr1[16][3] = 1;

	arr1[0][4] = 1;
	arr1[1][4] = 1;
	arr1[2][4] = 1;
	arr1[4][4] = 1;
	arr1[5][4] = 1;
	arr1[6][4] = 1;
	arr1[8][4] = 1;
	arr1[9][4] = 1;
	arr1[10][4] = 1;
	arr1[12][4] = 1;
	arr1[14][4] = 1;
	arr1[15][4] = 1;
	arr1[16][4] = 1;
	arr1[17][4] = 1;
	arr1[18][4] = 1;

	arr1[0][5] = 1;
	arr1[12][5] = 1;
	arr1[14][5] = 1;
	arr1[18][5] = 1;

	arr1[1][6] = 1;
	arr1[2][6] = 1;
	arr1[3][6] = 1;
	arr1[4][6] = 1;
	arr1[5][6] = 1;
	arr1[6][6] = 1;
	arr1[12][6] = 1;
	arr1[14][6] = 1;
	arr1[16][6] = 1;

	arr1[6][7] = 1;
	arr1[12][7] = 1;

	arr1[2][8] = 1;
	arr1[4][8] = 1;
	arr1[6][8] = 1;
	arr1[12][8] = 1;
	arr1[13][8] = 1;
	arr1[14][8] = 1;
	arr1[15][8] = 1;
	arr1[17][8] = 1;

	arr1[0][9] = 1;
	arr1[18][9] = 1;

	arr1[0][10] = 1;
	arr1[1][10] = 1;
	arr1[2][10] = 1;
	arr1[4][10] = 1;
	arr1[5][10] = 1;
	arr1[6][10] = 1;
	arr1[8][10] = 1;
	arr1[9][10] = 1;
	arr1[10][10] = 1;
	arr1[12][10] = 1;
	arr1[13][10] = 1;
	arr1[14][10] = 1;
	arr1[15][10] = 1;
	arr1[16][10] = 1;
	arr1[17][10] = 1;
	arr1[18][10] = 1;

	arr1[8][11] = 1;

	arr1[0][12] = 1;
	arr1[4][12] = 1;
	arr1[6][12] = 1;
	arr1[8][12] = 1;
	arr1[10][12] = 1;
	arr1[14][12] = 1;
	arr1[16][12] = 1;
	arr1[18][12] = 1;

	arr1[0][13] = 1;
	arr1[2][13] = 1;
	arr1[6][13] = 1;
	arr1[8][13] = 1;
	arr1[10][13] = 1;
	arr1[12][13] = 1;
	arr1[16][13] = 1;
	arr1[18][13] = 1;

	arr1[0][14] = 1;
	arr1[1][14] = 1;
	arr1[5][14] = 1;
	arr1[6][14] = 1;
	arr1[8][14] = 1;
	arr1[10][14] = 1;
	arr1[11][14] = 1;
	arr1[15][14] = 1;
	arr1[16][14] = 1;
	arr1[18][14] = 1;
#pragma endregion (arr)
	BoxSpawn(arr1);
	UE_LOG(LogTemp, Warning, TEXT("ThreeSpawnBox"));

}


// No Box
void ANStartBoxSpawn::BoxNoSpawn()
{

	// 하늘색 박스
#pragma region
	arr1[4][0] = 4;
	arr1[5][0] = 4;
	arr1[6][0] = 4;
	arr1[14][0] = 4;
	arr1[15][0] = 4;
	arr1[16][0] = 4;

	arr1[4][1] = 4;
	arr1[14][1] = 4;

	arr1[8][5] = 4;
	arr1[10][5] = 4;

	arr1[0][6] = 4;
	arr1[7][6] = 4;
	arr1[8][6] = 4;
	arr1[9][6] = 4;
	arr1[10][6] = 4;
	arr1[11][6] = 4;
	arr1[17][6] = 4;
	arr1[18][6] = 4;

	arr1[0][7] = 4;
	arr1[8][7] = 4;
	arr1[10][7] = 4;
	arr1[18][7] = 4;

	arr1[0][8] = 4;
	arr1[1][8] = 4;
	arr1[7][8] = 4;
	arr1[8][8] = 4;
	arr1[9][8] = 4;
	arr1[10][8] = 4;
	arr1[11][8] = 4;
	arr1[18][8] = 4;

	arr1[8][9] = 4;
	arr1[10][9] = 4;

	arr1[4][13] = 4;
	arr1[14][13] = 4;

	arr1[2][14] = 4;
	arr1[3][14] = 4;
	arr1[4][14] = 4;
	arr1[12][14] = 4;
	arr1[13][14] = 4;
	arr1[14][14] = 4;
#pragma endregion (arr)
	// 철 박스
	for (int row = 0; row < 19; row++)
	{
		for (int col = 0; col < 15; col++)
		{

			// 홀수 열에 있는 곳에만 상자 생성 X
			if (col % 2 == 1 && row % 2 == 1)
			{
				arr1[row][col] = 5;


				FVector SpawnLocation = FVector(row * 100, col * 100, 100);
				AActor* SpawnActor = GetWorld()->SpawnActor<AActor>(NoBoxClass, SpawnLocation, FRotator::ZeroRotator);

			}
		}
	}


	// 테스트
	//BoxSpawn(arr1);
}


void ANStartBoxSpawn::OnRep_randomValue()
{
	UE_LOG(LogTemp, Warning, TEXT("ggggggggggggg"));
	if (!HasAuthority()) // 이 객체가 클라이언트인지 확인
	{

	}
	/* if (randomValue == 0)
		 FirstSpawnBox();
	 else if (randomValue == 1)
		 TwoSpawnBox();
	 else
		 ThreeSpawnBox();

	 Timer60();*/
}


void ANStartBoxSpawn::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 여기서 동기화할 프로퍼티들을 추가합니다.
	DOREPLIFETIME(ANStartBoxSpawn, randomValue);
}


